#include "serverFirmware.h"


ServerFirmware *ServerFirmware::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerFirmware *ServerFirmware::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerFirmware();
    }
    return instance;
}


/*******************************************************************************
* Function Name  :  ServerFirmware
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerFirmware::ServerFirmware(QObject *parent) : QObject(parent)
{
    totalSize = 0;
    readySize = 0;
    index = 0;
}

/*******************************************************************************
* Function Name  :  check_update
* Description    :  收到服务器返回设备初始化数据
* Input          :  QString version 目前的版本
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_check_update(QString versionFirmware, QString versionSystem, QString versionType)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_checkUpdate_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_checkUpdate_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_checkUpdate_failed()));

    // 初始化检查更新的地址
    QString strCheck = QString(URL_SERVER) + QString(REQUEST_CHECK_UPDATE) + "?version_firmware=" + versionFirmware + "&version_system=" + versionSystem + "&version_type=" + versionType;

    qDebug()<<strCheck;

    // 获取设备信息
    QUrl urlCheckUpdate = QUrl::fromEncoded(strCheck.toAscii());

    // 获取设备信息
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setUrl(urlCheckUpdate);
    networkManage->get_request(request);
}

/*******************************************************************************
* Function Name  :  slot_timeout_checkUpdate
* Description    :  检查更新超时处理
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_checkUpdate_failed()
{
    qDebug()<<"Check Update Timeout!!!";
    emit signal_checkFirmware_failed();
}

/*******************************************************************************
* Function Name  :  analyse_reply_checkUpdate
* Description    :  解析返回JSON数据
* Input          :  QString repliData
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_checkUpdate_success(QString replyData)
{
    InfoFirmware firmware;
    File file;
    firmware.size = 0;
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);

    if(NULL == root)
    {
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(1002 == code->valueint)
    {
        emit signal_checkFirmware_success(firmware);
    }
    else if(0 != code->valueint)
    {
        emit signal_checkFirmware_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        firmware.type = get_validString(data,"type");
        cJSON *fileItem = cJSON_GetObjectItem(data,"file");
        if(fileItem)
        {
            // 获取文件个数
            int size = cJSON_GetArraySize(fileItem);

            qDebug()<<"analyse!!!!!!!!!"<<size;;
            for(int i = 0; i < size; ++i)
            {
                // 循环解析
                cJSON *fileJson = cJSON_GetArrayItem(fileItem,i);
                if(fileJson)
                {

                    char *everyFile= cJSON_Print(fileJson);
                    qDebug()<<QString(everyFile);

                    cJSON *imgItem = cJSON_Parse(everyFile);
                    file.url = get_validString(imgItem,"url");
                    file.md5 = get_validString(imgItem,"file_md5");
                    file.size = get_validNum(imgItem, "filesize");
                    if(firmware.type == "system")
                    {
                        file.name = QString("/tmp/upgrade/%1").arg(get_validString(imgItem,"filename"));
//                        file.name = QString("/home/rcl/%1").arg(get_validString(imgItem,"filename"));
                    }
                    else
                    {
                        file.name = QString("/tmp/%1").arg(get_validString(imgItem,"filename"));
                    }
                    firmware.size += file.size;
                    firmware.listFile.append(file);
                }
            }

        }
        firmware.version   = get_validString(data, "version");
        firmware.desc      = get_validString(data, "desc");
        firmware.createdTime = get_validString(data,"created_at");
        emit signal_checkFirmware_success(firmware);
    }
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerFirmware::get_validString(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return QString(cJSON_GetObjectItem(item, cField)->valuestring);
    }
    return "";
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
int ServerFirmware::get_validNum(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return cJSON_GetObjectItem(item, cField)->valuedouble;
    }
    return 0;
}

/*******************************************************************************
* Function Name  :  slot_download_firmware
* Description    :  下载新固件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_download_file(InfoFirmware firmware)
{
    qDebug()<<"**********************************Download Start***************************************"<<QThread::currentThreadId();
    this->newFirmware = firmware;
    totalSize = 0;
    readySize = 0;
    index     = 0;
//    int i;
//    for(i = 0; i < newFirmware.listFile.length(); ++i)
//    {
//        totalSize += newFirmware.listFile.at(i).size;
//    }
    totalSize = firmware.size;

    // 开始下载文件
    start_download(newFirmware.listFile.at(index));
}

/*******************************************************************************
* Function Name  :  start_download
* Description    :  开始下载
* Input          :  QString filePath 文件路径
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::start_download(File file)
{
    NetworkManage *networkManage = new NetworkManage();
    connect(networkManage, SIGNAL(signal_download_success(QString)), this, SLOT(slot_download_success(QString)));
    connect(networkManage, SIGNAL(signal_download_process(qint64,qint64)), this, SLOT(slot_download_process(qint64,qint64)));
    connect(networkManage, SIGNAL(signal_download_timeout()), this, SLOT(slot_download_failed()));
    connect(networkManage, SIGNAL(signal_download_failed()), this, SLOT(slot_download_failed()));
    networkManage->download_file(file);
}

/*******************************************************************************
* Function Name  :  slot_download_process
* Description    :  下载进度更新函数
* Input          :  qint64 bytesGot 已下载字节数
*                :  qint64 bytesTotal   总字节数
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_download_process(qint64 bytesGot,qint64 bytesTotal)
{
    // 更新当前的下载进度
    emit signal_download_process(bytesGot + readySize,totalSize);
}

/*******************************************************************************
* Function Name  :  slot_download_finished
* Description    :  下载结束
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_download_success(QString md5)
{
    readySize += newFirmware.listFile.at(index).size;

    qDebug()<<newFirmware.listFile.at(index).md5;
    qDebug()<<md5;
    if(newFirmware.listFile.at(index++).md5 == md5)
    {
        if(newFirmware.listFile.length() == index)
        {
            emit signal_download_success();
        }
        else
        {
            start_download(newFirmware.listFile.at(index));
        }
    }
    else
    {
        qDebug()<<"MD5 ________________";
        emit signal_download_failed();
    }
    qDebug()<<"**********************************Download End****************************************";
}

/*******************************************************************************
* Function Name  :  slot_download_failed
* Description    :  下载超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerFirmware::slot_download_failed()
{
    qDebug()<<"DOWNLOAD FAILED!!!!!!!!!!!!!!!";
    emit signal_download_failed();
}







