#include "driverTvocCo2e.h"

DriverTvocCo2e *DriverTvocCo2e::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTvocCo2e *DriverTvocCo2e::getInstance()
{
    if(0 == instance)
    {
        instance = new DriverTvocCo2e();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DriverTvocCo2e
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTvocCo2e::DriverTvocCo2e(QObject *parent)
    : QObject(parent)
{
    data_init();
    connect_init();
    qDebug()<<"baseline@@@@@@@@@@@@@@"<<get_baseline();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::data_init()
{
    timerClean = new QTimer();
    timerClean->setSingleShot(true);
    timerClean->setInterval(15*60*1000);

    timerStore = new QTimer();
    timerStore->setInterval(60*60*1000);

    timerOperation = new QTimer();
    timerOperation->setSingleShot(true);
    timerOperation->setInterval(60*60*1000*12);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::connect_init()
{
    connect(timerStore, SIGNAL(timeout()), this, SLOT(slot_store_baseline()));
    connect(timerOperation, SIGNAL(timeout()), this, SLOT(slot_store_baseline()));
    connect(timerClean, SIGNAL(timeout()), this, SLOT(slot_iaq_init()));
}

/*******************************************************************************
* Function Name  :  start
* Description    :  开始
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::start()
{
    QString baseline;
    long timestamp;
    int i = 0;
    iaq_init();

//    // 此时的时间戳
//    time_t nowTimestamp = QDateTime::currentDateTime().toTime_t();

//    // 获取当前的baseline
//    if(load_baseline_from_disk(baseline, timestamp) && ((nowTimestamp - timestamp) < BASELINE_VALID_S))
//    {
//        for(; i < 3; ++i)
//        {
//            // 设置当前的baseline
//            set_baseline(baseline);
//            if(get_baseline() == baseline)
//            {
//                break;
//            }
//        }
//        timerStore->start();
//    }
//    else
//    {
//        // clean
//        timerClean->start();
//        timerOperation->start();
//    }
}

/*******************************************************************************
* Function Name  :  update_data
* Description    :  刷新数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::update_data(float &tvoc, float &co2e)
{
    QFile file(DEVICE_TVOCCO2E);
    QStringList listFile;
    QString strFile,strTvoc,strCo2e;

    if(!file.exists())
    {
        tvoc = ERROR_DATA;
        co2e = ERROR_DATA;
        return;
    }

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);

        // 取得 温度值
        strFile     = stream.readAll();
        listFile = strFile.split("\n");
        if(listFile.length() >= 2)
        {
            strTvoc = (((listFile.at(0).trimmed()).split(" ")).at(1)).trimmed();
            strCo2e  = (((listFile.at(1).trimmed()).split(" ")).at(1)).trimmed();

            tvoc=strTvoc.toFloat();
            co2e=strCo2e.toFloat();

            // 对数据进行四舍五入，保留两位小数
    //        tvoc = QString::number(tvoc, 'f', 2).toFloat();
    //        co2e  = QString::number(co2e, 'f', 2).toFloat();

            if((tvoc >= 1000) || ( tvoc < 0))
            {
                tvoc = ERROR_DATA;
            }
            if((co2e > 10000)||(co2e < 400))
            {
                co2e = ERROR_DATA;
            }
        }
        file.close();
    }
    else
    {
        tvoc = ERROR_DATA;
        co2e  = ERROR_DATA;
    }
}

/*******************************************************************************
* Function Name  :  load_baseline_from_disk
* Description    :  载入新的baseline
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool DriverTvocCo2e::load_baseline_from_disk(QString &baseline, long &timestamp)
{
    QFile file(FILE_BASELINE);
    QStringList listFile;

    // 文件温度 湿度 内容
    QString strFile,strBaseline,strTimestamp;

    // 若此时文件不存在
    if(!file.exists())
    {
        return false;
    }

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);

        strFile     = stream.readAll();

        listFile = strFile.split("\n");
        if(listFile.length() >= 2)
        {
            strBaseline = (((listFile.at(0)).split("=")).at(1)).trimmed();
            strTimestamp  = (((listFile.at(1)).split("=")).at(1)).trimmed();
            baseline = strBaseline;
            timestamp = strTimestamp.toInt();
        }
        file.close();
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  store_baseline_to_disk
* Description    :  存储baseline到本地
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::store_baseline_to_disk(QString baseline, long timestamp)
{
    // 存储一次baseline
    QFile file(FILE_BASELINE);

    if(file.open(QFile::WriteOnly))
    {
        //声明并初始化数据流对象
        QTextStream stream(&file);
        stream<<QString("baseline = %1").arg(baseline)<<"\n";
        stream<<QString("timestamp = %1").arg(timestamp);
        file.close();
    }
}

/*******************************************************************************
* Function Name  :  slot_iaq_init
* Description    :  传感器初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::slot_iaq_init()
{
    iaq_init();
}

/*******************************************************************************
* Function Name  :  iaq_init
* Description    :  传感器初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::iaq_init()
{
    // 初始化
    system(qPrintable("echo \"init\" > /sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/iaq_init"));
}

/*******************************************************************************
* Function Name  :  set_baseline
* Description    :  设置baseline
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::set_baseline(QString baseline)
{
    return ;
    QFile file(FILE_IAQ_BASELINE);

    if(file.open(QFile::WriteOnly))
    {
        //声明并初始化数据流对象
        QTextStream stream(&file);
        stream<<baseline;
        file.close();
    }
}

/*******************************************************************************
* Function Name  :  get_baseline
* Description    :  获取baseline
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString DriverTvocCo2e::get_baseline()
{
    QFile file("/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/iaq_baseline");
    QString strFile;

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);

        // 取得 温度值
        strFile     = stream.readAll();

        file.close();
    }

    return strFile.trimmed();
}

/*******************************************************************************
* Function Name  :  slot_store_baseline
* Description    :  刷新此时的baseline到本地存储
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTvocCo2e::slot_store_baseline()
{
    if(!timerStore->isActive())
    {
        timerStore->start();
    }

    // 获取最新的baseline和当前时间戳
    QString baseline = get_baseline().trimmed();
    long timestamp = QDateTime::currentDateTime().toTime_t();
    store_baseline_to_disk(baseline,timestamp);
}


