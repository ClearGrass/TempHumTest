#include "funcFirmware.h"

FuncFirmware *FuncFirmware::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例模式
* Input          :  None
* Output         :  None
* Return         :  单例
*******************************************************************************/
FuncFirmware* FuncFirmware::getInstance()
{
    if(NULL == instance)
    {
        instance = new FuncFirmware();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FuncFirmware
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncFirmware::FuncFirmware()
    :QThread()
{
    qRegisterMetaType<InfoFirmware>("InfoFirmware");
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::data_init()
{
    serverFirmware = ServerFirmware::getInstance();

    // 重试检查更新定时器
    timerRetry = new QTimer();
    timerRetry->setInterval(RETRY_CHECK_UPDATE_INTERVAL);
    timerRetry->setSingleShot(true);

    // 日常检查更新定时器
    timerCheck = new QTimer();
    timerCheck->setInterval(CHECK_UPDATE_INTERVAL);
    timerCheck->setSingleShot(true);

    isConnected = false;
    status  = DISDOWNLOAD;

    readySize = 0;
    totalSize = 0;

}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::connect_init()
{
    // 返回检查更新收到的固件信息
    connect(serverFirmware, SIGNAL(signal_checkFirmware_success(InfoFirmware)), this, SLOT(slot_check_success(InfoFirmware)));
    connect(serverFirmware, SIGNAL(signal_checkFirmware_failed()) ,this, SLOT(slot_check_failed()));
    connect(serverFirmware, SIGNAL(signal_download_process(qint64,qint64)), this, SLOT(slot_download_process(qint64,qint64)));
    connect(serverFirmware, SIGNAL(signal_download_success()), this, SLOT(slot_download_success()));
    connect(serverFirmware, SIGNAL(signal_download_failed()), this, SLOT(slot_download_failed()));
    connect(this, SIGNAL(signal_check_update(QString,QString,QString)), serverFirmware, SLOT(slot_check_update(QString,QString,QString)));
    connect(this, SIGNAL(signal_download_file(InfoFirmware)), serverFirmware, SLOT(slot_download_file(InfoFirmware)));
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_check_auto()));
    connect(timerCheck, SIGNAL(timeout()), this, SLOT(slot_check_auto()));
}



/*******************************************************************************
* Function Name  :  slot_check_auto
* Description    :  自动检查更新
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_check_auto()
{
    if(isConnected)
    {
        // 检查更新
        emit signal_check_update(get_version_firmware(),get_version_system(),"release");
    }
}

/*******************************************************************************
* Function Name  :  slot_checkRelease_Manual
* Description    :  手动检测发布版
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_checkRelease_manual()
{
    // 若此时已有网络
    if(isConnected)
    {
        // 若系统在准备更新，取消
        if(timerCheck->isActive())
        {
            timerCheck->stop();
        }
        if(timerRetry->isActive())
        {
            timerRetry->stop();
        }

        // 检查更新
        emit signal_check_update(get_version_firmware(),get_version_system(),"release");
        isManual = true;
    }
    else
    {
        // 发送无网信号
        emit signal_net_disconnect();
    }
}


/*******************************************************************************
* Function Name  :  slot_checkDebug_Manual()
* Description    :  手动检测测试版
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_checkDebug_manual()
{
    // 若此时已有网络
    if(isConnected)
    {
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Debug";

        // 检查更新
        emit signal_check_update(FIRMWARE_VERSION,get_version_system(),"debug");
        isManual = true;
    }
    else
    {
        // 发送无网信号
        emit signal_net_disconnect();
    }
}


/*******************************************************************************
* Function Name  :  slot_wifiStatus_changed
* Description    :  WiFi状态改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_net_connected()
{
    isConnected = true;

    // 若此时检查定时器未开
    emit signal_check_update(FIRMWARE_VERSION,get_version_system(),"release");
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_net_disconnected()
{
    isConnected = false;
    if(timerCheck->isActive())
    {
        timerCheck->stop();
    }

    if(timerRetry->isActive())
    {
        timerRetry->stop();
    }
}

/*******************************************************************************
* Function Name  :  slot_feedback_checkUpdate
* Description    :  有新的固件版本，并返回
* Input          :  Firmware firmaware 新的固件
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_check_success(InfoFirmware firmware)
{
    newFirmware = firmware;

    // 若固件不为空
    if(!firmware.isEmpty())
    {
        // 发送固件信息到前台，告知新的版本已有更新,发送新版本的版本号
        emit signal_checkFirmware_success(firmware.version,firmware.desc);

        // 若此时为自动更新的结果
        if(!isManual)
        {
            // 开始自动下载文件
            slot_download_auto();
        }
    }
    else
    {
        // 发送信息给前台，告知无固件更新
        emit signal_checkFirmware_success();
    }

    timerCheck->start();
}

/*******************************************************************************
* Function Name  :  slot_checkUpdate_fail
* Description    :  检查固件版本失败，重新检查
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_check_failed()
{
    qDebug()<<"Check Firmware Failed!!!";

    // 若是手动检测
    if(isManual)
    {
        // 若此时已联网
        if(isConnected)
        {
            // 开始定时器，过一段时间重新检测
            emit signal_net_abnormal();
        }
        else
        {
            emit signal_net_disconnect();
        }
    }
    // 若此时的网络连接
    if(isConnected)
    {
        timerRetry->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_start_download
* Description    :  开始下载
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_download_auto()
{
    if(newFirmware.isEmpty())
    {
        return;
    }
    QDir *upgrade = new QDir;
    if(SYSTEM == newFirmware.type)
    {
        // 判断/tmp/upgrade文件夹是否存在
        if(!upgrade->exists("/tmp/upgrade"))
        {
            upgrade->mkdir("/tmp/upgrade");
        }
    }

    // 开始下载文件
    emit signal_download_file(newFirmware);

    // 此时的状态为正在下载文件
    status = DOWNLOADING;
}

/*******************************************************************************
* Function Name  :  slot_download_manual
* Description    :  手动下载
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_download_manual()
{
    if(!isConnected)
    {
        // 发送无网信号
        emit signal_net_disconnect();
        return ;
    }

    isManual = true;

    // 若此时文件已经下载下来
    if(DOWNLOADED == status)
    {
        // 发送下载成功信号，开始安装
        emit signal_download_process(readySize, totalSize);
        qDebug()<<readySize<<totalSize;
        emit signal_download_success();
        if(SYSTEM == newFirmware.type)
        {
            //
            system(qPrintable("kill -9 `ps -ef|grep \"watchdog.sh\"|grep -v \"grep\"|awk '{print $2}'`"));

            // 执行脚本文件
            QFile sFile("./OTA.sh");
            if(sFile.exists())
            {
                system(qPrintable("./OTA.sh"));
            }
            else
            {
                emit signal_install_failed();
            }
        }
        else
        {
            system(qPrintable(QString("chmod 777 %1").arg(newFirmware.listFile.at(0).name)));
            system(qPrintable(QString("tar -zxvf %1 -C /bin/qtapp/").arg(newFirmware.listFile.at(0).name)));
            system(qPrintable(QString("rm %1").arg(newFirmware.listFile.at(0).name)));
            system(qPrintable("chmod 777 *"));
            system_reboot();
        }
    }
    else if(DOWNLOADING == status)
    {
        // 更新此时的进度条
        emit signal_download_process(readySize, totalSize);
    }
    else
    {
        qDebug()<<"START MANUAL DOWNLOAD!!!";

        QDir *upgrade = new QDir;
        if(SYSTEM == newFirmware.type)
        {
            // 判断/tmp/upgrade文件夹是否存在
            if(!upgrade->exists("/tmp/upgrade"))
            {
                upgrade->mkdir("/tmp/upgrade");
            }
        }

        // 开始下载文件
        emit signal_download_file(newFirmware);
        status = DOWNLOADING;
    }
}

/*******************************************************************************
* Function Name  :  slot_feedback_download_process
* Description    :  更新目前下载进度
* Input          :  qint64 bytesGot 已下载字节数
*                :  qint64 bytesTotal   总共字节数
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_download_process(qint64 bytesGot, qint64 bytesTotal)
{
    readySize = bytesGot;
    totalSize = bytesTotal;

    if(isManual)
    {
        // 发送此时下载进度
        emit signal_download_process(bytesGot,bytesTotal);
    }
}

/*******************************************************************************
* Function Name  :  slot_feedback_download_success
* Description    :  下载成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_download_success()
{
    qDebug()<<"DOWNLOAD SUCCESS!!!!!!!!!!!!!!";

    // 下载完成
    status = DOWNLOADED;
    if(isManual)
    {
        // 发送下载成功信号
        emit signal_download_success();

        // 下载结束等待安装
        slot_download_manual();
    }
}

/*******************************************************************************
* Function Name  :  slot_feedback_download_fail
* Description    :  发送下载失败信号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::slot_download_failed()
{
    // 将状态置为未下载
    status = DISDOWNLOAD;
    if(isManual)
    {
        // 发送下载失败信号
        if(isConnected)
        {
            emit signal_net_abnormal();
        }
        else
        {
            emit signal_net_disconnect();
        }
    }
    else
    {
        if(isConnected)
        {
            slot_download_auto();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_reboot
* Description    :  安装结束后系统重启
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncFirmware::system_reboot()
{
    // 执行脚本文件
    system(qPrintable("./firmwareUpdate.sh"));
    emit signal_device_reboot();
}

/*******************************************************************************
* Function Name  :  get_version
* Description    :  获取此时的版本号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
InfoVersion FuncFirmware::get_version()
{
    InfoVersion infoVersion;
    infoVersion.firmware = get_version_firmware();
    infoVersion.system   = get_version_system();
    return infoVersion;
}

/*******************************************************************************
* Function Name  :  get_version_system
* Description    :  获取系统版本
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FuncFirmware::get_version_firmware()
{
    return QString(FIRMWARE_VERSION);
}

/*******************************************************************************
* Function Name  :  get_version_system
* Description    :  获取系统版本
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FuncFirmware::get_version_system()
{
    QFile file("/bin/qtapp/IMG_Version");
    QStringList listStr;
    QString strRead;
    QString version = "999.0";
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);

        //读取文件内容
        strRead = stream.readAll();
        listStr = strRead.split(" ");
        version = listStr.at(0).trimmed();

        //关闭文件
        file.close();
    }
    return version;
}


