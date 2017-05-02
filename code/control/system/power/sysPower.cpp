#include "sysPower.h"

SysPower *SysPower::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysPower *SysPower::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysPower();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysPower
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysPower::SysPower(QObject *parent)
    : QObject(parent)
{
    data_init();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysPower::data_init()
{
    driverPM25 = DriverPM25::getInstance();
    driverTime = DriverTime::getInstance();
}

/*******************************************************************************
* Function Name  :  ready_powerOff
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysPower::ready_powerOff()
{
#ifdef Bran_R8
    driverPM25->power_off();
    driverTime->set_rtc();
#endif
}

/*******************************************************************************
* Function Name  :  device_powerOff
* Description    :  关机操作
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysPower::device_powerOff()
{
    int res;

#ifdef Bran_R8

    // 关机前的准备
    ready_powerOff();
    usleep(100);
    res = system("poweroff");
#endif
    qDebug() <<  "exec poweroff  :" << res << endl;
}

/*******************************************************************************
* Function Name  :  device_reboot
* Description    :  重启操作
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysPower::device_reboot()
{
    int res;
#ifdef Bran_R8
    // 关机前的准备
    ready_powerOff();
    usleep(100);
    res = system("reboot");
#endif

    qDebug() <<  "exec reboot  :" << res << endl;
}


