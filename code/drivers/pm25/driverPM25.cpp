#include "driverPM25.h"

DriverPM25 *DriverPM25::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverPM25 *DriverPM25::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverPM25();

    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DriverPM25
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverPM25::DriverPM25()
    :QThread()
{
    powerStatus = false;

#ifdef Bran_R8
    power_off();
#endif
    this->start();
    moveToThread(this);

}

void DriverPM25::run()
{
    this->exec();
}


void DriverPM25::start_sampling()
{
    mutex.lock();
#ifdef Bran_R8
    open_uart("ttyS1");
    power_on();
#endif
    mutex.unlock();
}

void DriverPM25::stop_sampling()
{
    mutex.lock();
#ifdef Bran_R8
    power_off();
    close_uart();
#endif
    emit signal_update_dataPM25(-1);
    mutex.unlock();
}

/*******************************************************************************
* Function Name :   openUart
* Description   :   打开串口
* Input         :   const QString &uartName 要打开的串口名
* Output        :   None
* Return        :   true false
*******************************************************************************/
bool DriverPM25::open_uart(const QString &uartName)
{
    // 配置参数
    PortSettings uartSettings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 100};

    // 设置串口参数，中断事件 9600 8 N 1
    uartPort = new QextSerialPort(uartName, uartSettings, QextSerialPort::EventDriven);
    qDebug(" open uart");
    // 打开串口
    if (uartPort->open(QIODevice::ReadWrite) == true)
    {
        // 连接接收槽
        connect(uartPort, SIGNAL(readyRead()), this, SLOT(slot_recive_pmData()));
        qDebug("UART READY!!!");
        return true;
    }
    else
    {
        // 打开失败
        qDebug() << "device failed to open:" << uartPort->errorString();
        return false;
    }

    return true;

}// End of bool PM25Uart::openUart(const QString &uartName)

/*******************************************************************************
* Function Name :   closeUart
* Description   :   关闭串口
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void DriverPM25::close_uart(void)
{
    uartPort->close();

    // 结束处理
    qDebug() << "UART close!";

}// End of void PM25Uart::closeUart(void)

/*******************************************************************************
* Function Name :   recive_pmData
* Description   :   串口接受槽
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void DriverPM25::slot_recive_pmData(void)
{
    float fPM2_5Data = ERROR_DATA;
    float fPM5Data   = ERROR_DATA;
    float fPM10Data  = ERROR_DATA;

    if(powerStatus == false)
    {
        qDebug("ERROR<<电源已关，仍有数据反馈!!!");
        power_off();
        return;
    }

    // 测试中断时间
    //    qDebug() << PM25TimeStamp.elapsed () << "ms";

#ifdef Bran_R8

    // 声明接受数组和长度
    QByteArray rxDataArray;
    int        iRxDataNum ;
    // 获取接受字节数
    iRxDataNum = uartPort->bytesAvailable();

    // 有数据
    if (iRxDataNum)
    {
        // 分配空间 读取缓存数据
        rxDataArray.resize(iRxDataNum);
        uartPort->read(rxDataArray.data(), rxDataArray.size());

        // 打印
        // 处理PM2.5数据
        if ((iRxDataNum == 60) && (rxDataArray.at(0) == 0x5a) && (rxDataArray.at(1) == 0xa5))
        {
            data_transform(&rxDataArray, &fPM2_5Data, &fPM5Data, &fPM10Data);

            // 数据正确
            if (fPM2_5Data == ERROR_DATA)
            {
                emit signal_update_dataPM25(ERROR_DATA);
            }
            else
            {
                // 临时改动
                fPM2_5Data = qRound(fPM2_5Data/100.0);
                emit signal_update_dataPM25(fPM2_5Data);
            }

            if(fPM5Data == ERROR_DATA)
            {
                emit signal_update_dataPM5(ERROR_DATA);
            }
            else
            {
                fPM5Data = qRound(fPM5Data/100.0);
                emit signal_update_dataPM5(fPM5Data);
            }

            if(fPM10Data == ERROR_DATA)
            {
                emit signal_update_dataPM10(ERROR_DATA);
            }
            else
            {
                fPM10Data = qRound(fPM10Data/100.0);
                emit signal_update_dataPM10(fPM10Data);
            }
        }
        else
        {
            qDebug() << "UART Rx Data Num =" << rxDataArray.size();
            char *data = rxDataArray.data();
            while(*data)
            {
                qDebug()<<*data;
                ++data;
            }
        }

    }
    else
    {
        qDebug() << "UART 无数据错误中断！";
    }

#else

    fPM2_5Data = (qrand() % 500) * 1.0;
    // 数据处理
    //    data_handle(fPM2_5Data);

#endif


}// End of void PM25Uart::rxDataReady(void)

/*******************************************************************************
* Function Name :   PM2_5_dataHandle
* Description   :   PM2.5数据处理
* Input         :   None
* Output        :   None
* Return        :   PM2.5值
*******************************************************************************/
void DriverPM25::data_transform(QByteArray *DataPM2_5, float *fPM2_5, float *fPM5, float *fPM10)
{
    // Check值
    unsigned short usCheck = 0;
    int i;
    *fPM2_5 = ERROR_DATA;
    *fPM5   = ERROR_DATA;
    *fPM10  = ERROR_DATA;

    // 获取校验值
    for (i = 0; i < 56; i += 4)
    {
        usCheck += DataPM2_5->at(i) + DataPM2_5->at(i + 1) + (DataPM2_5->at(i + 2) << 8) +  (DataPM2_5->at(i + 3) << 16);
    }

    // 通过验证
    if (usCheck == (DataPM2_5->at(58) << 8) + DataPM2_5->at(59))
    {
        // 提取PM2.5数据
        *fPM2_5 = (DataPM2_5->at(19) << 24) + (DataPM2_5->at(18) << 16) + (DataPM2_5->at(17) << 8) + (DataPM2_5->at(16) << 0);

        // 提取PM5数据
        *fPM5   = (DataPM2_5->at(27) << 24) + (DataPM2_5->at(26) << 16) + (DataPM2_5->at(25) << 8) + (DataPM2_5->at(24) << 0);

        // 提取PM10数据
        *fPM10  = (DataPM2_5->at(35) << 24) + (DataPM2_5->at(34) << 16) + (DataPM2_5->at(33) << 8) + (DataPM2_5->at(32) << 0);
    }
    else
    {
        qDebug() << "PM2.5 CRC Error = " << usCheck;
    }


}// End of int PM2_5_dataHandle(QByteArray *DataPM2_5)


/*******************************************************************************
* Function Name :   power_on
* Description   :   打开PM2.5电源
* Input         :   None
* Output        :   None
* Return        :   true false
*******************************************************************************/
bool DriverPM25::power_on(void)
{
    qDebug("power on");
    // 打开文件
    QFile file(DEVICE_PM25);
    if (file.open(QFile::ReadWrite))
    {
        file.write("1");
        powerStatus = true;
        file.close();
        qDebug() << "Boost_En on";
        return true;
    }
    else
    {
        qDebug() << file.errorString();
        return false;
    }

}// End of bool PM25Uart::power_on(void)

/*******************************************************************************
* Function Name :   power_off
* Description   :   关闭PM2.5电源
* Input         :   None
* Output        :   None
* Return        :   true false
*******************************************************************************/
bool DriverPM25::power_off(void)
{

    // 打开文件
    QFile file(DEVICE_PM25);
    if (file.open(QFile::ReadWrite))
    {
        file.write("0");
        powerStatus = false;
        file.close();
        qDebug() << "Boost_En off";
        return true;
    }
    else
    {
        qDebug() << file.errorString();
        return false;
    }

}// End of bool PM25Uart::power_off(void)
