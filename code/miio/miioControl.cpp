#include "miioControl.h"

MiioControl *MiioControl::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
MiioControl *MiioControl::getInstance()
{
    if(instance == 0)
    {
        instance = new MiioControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  MiioControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
MiioControl::MiioControl(QObject *parent)
    : QThread()
{
    qRegisterMetaType<BindKey>("BindKey");
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
void MiioControl::data_init()
{
    timer = new QTimer();
    timer->setInterval(6000);
    timer->setSingleShot(true);

    miioSocket = new QTcpSocket(this);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::connect_init()
{
    connect(miioSocket, SIGNAL(readyRead()), this, SLOT(slot_miio_dataRecived()));
    connect(miioSocket, SIGNAL(connected()), this, SLOT(slot_miio_connected()));
    connect(miioSocket, SIGNAL(disconnected()), this, SLOT(slot_miio_disconnect()));
    connect(timer, SIGNAL(timeout()), this, SIGNAL(signal_updateBindKey_failed()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::run()
{
    miioSocket->abort();
    miioSocket->connectToHost(QHostAddress::LocalHost, MIIO_SERVER_PORT);
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_recive_msg
* Description    :  收到米家客户端消息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::slot_miio_dataRecived()
{
    QByteArray ba = miioSocket->readAll();
    QString msg = QVariant(ba).toString();

    qDebug()<<"RECIVE:"<<msg;

    if(msg.contains("method"))
    {
        // 若消息包含事件，则认定为服务端主动发送数据
        analyse_method(msg);
    }
    else if((msg.contains("result")) || (msg.contains("error")))
    {
        // 若消息包含result或者error则为服务端被动反馈
        analyse_result(msg);
    }
}

/*******************************************************************************
* Function Name  :  slot_miio_connected
* Description    :  miio连接成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::slot_miio_connected()
{
    isConnected = true;
    qDebug()<<"MIIO CONNECTED!!!";
}

/*******************************************************************************
* Function Name  :  slot_miio_disconnect
* Description    :  miio断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::slot_miio_disconnect()
{
    isConnected = false;
    qDebug()<<"MIIO DISCONNECT"<<miioSocket->socketDescriptor();
}

/*******************************************************************************
* Function Name  :  send_msg
* Description    :  发送消息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::send_msg(QString msg)
{
    qDebug()<<"SEND:"<<msg;
    miioSocket->write(msg.toStdString().c_str(), strlen(msg.toStdString().c_str())) ;
}

/*******************************************************************************
* Function Name  :  slot_request_bindkey
* Description    :  获取绑定码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::slot_request_bindkey()
{
    QString msg = "{\"id\":1,\"method\":\"_sync.get_bind_key\",\"params\":{}}";
    send_msg(msg);
    timer->start();
}

/*******************************************************************************
* Function Name  :  analyse_method
* Description    :  分析方法
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::analyse_method(QString msg)
{
    QByteArray ba = msg.toLatin1();
    char *cMsg = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cMsg);
    if(NULL == root)
    {
        return;
    }

    QString method = get_validString(root, "method");
    qDebug()<<"######################################";
    cJSON  *params  = cJSON_GetObjectItem(root, "params");
    qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

    // 若此时收到关于app绑定的消息
    if(method.contains("miIO.bind_stat"))
    {
        // 若返回code不为0(正常)，返回
        cJSON *id     = cJSON_GetObjectItem(root, "id");

        // 绑定状态为已连接
        if(params)
        {
            int stat = get_validNum(params,"stat");
            emit signal_update_bindStatus(stat);
            reply_result_OK(id->valueint);
        }
    }
    else if(method.contains("miIO.restore"))
    {
        // 若返回code不为0(正常)，返回
        cJSON *id     = cJSON_GetObjectItem(root, "id");

        // 绑定状态为断开连接
        emit signal_update_bindStatus(0);
        reply_result_OK(id->valueint);

        this->sleep(2);

        // 重启米家
        miio_restore();
    }

}

/*******************************************************************************
* Function Name  :  analyse_msg
* Description    :  分析回复
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::analyse_result(QString msg)
{
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = msg.toLatin1();
    char *cMsg = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cMsg);
    if(NULL == root)
    {
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *id = cJSON_GetObjectItem(root, "id");
    switch (id->valueint)
    {
    case METHOD_GET_BIND_KEY:
        timer->stop();
        if(msg.contains("result"))
        {
            cJSON *result = cJSON_GetObjectItem(root, "result");
            BindKey bindKey;
            if(result)
            {
                bindKey.value = get_validString(result, "bind_key");
                bindKey.expire = get_validNum(result, "expire");
            }
            emit signal_update_bindKey(bindKey);
        }
        else if(msg.contains("error"))
        {
            cJSON *error = cJSON_GetObjectItem(root, "error");
            if(error)
            {
            }
            qDebug()<<error->valuestring;
            emit signal_updateBindKey_failed();
        }
        break;
    default:
        break;
    }
}

/*******************************************************************************
* Function Name  :  miio_restore
* Description    :  米家客户端重启
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::miio_restore()
{
    // 运行重启米家客户端的重启
    system(qPrintable("/bin/qtapp/miio/miio_restore.sh"));

    // delay3秒
    this->sleep(3);
    miioSocket->abort();
    miioSocket->connectToHost(QHostAddress::LocalHost, MIIO_SERVER_PORT);
}

/*******************************************************************************
* Function Name  :  reply_result_OK
* Description    :  返回成功结果
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MiioControl::reply_result_OK(int id)
{
    QString result = QString("{\"result\":\"OK\",\"id\":%1}").arg(id);
    send_msg(result);
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString MiioControl::get_validString(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return QString(cJSON_GetObjectItem(item, cField)->valuestring).trimmed();
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
int MiioControl::get_validNum(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return cJSON_GetObjectItem(item, cField)->valueint;
    }
    return 0;
}
