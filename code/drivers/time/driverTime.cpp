/*******************************************************************************
* File Name          :  DriverTime.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  10/28/2016
* Description        :  time驱动
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <QDebug>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <QProcess>

#include "driverTime.h"
#include "globalVariable.h"




/* Private variables ---------------------------------------------------------*/
DriverTime *DriverTime::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTime* DriverTime::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverTime();
    }
    return instance;
}

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
*                           陆超@2016-10-28
* Function Name  :  DriverTime
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTime::DriverTime(void)
{

#ifdef Bran_R8
#endif

}// End of DriverTime::DriverTime()

/*******************************************************************************
*                           陆超@2016-10-28
* Function Name :   ~DriverTime
* Description   :   析构函数
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
DriverTime::~DriverTime(void)
{
#ifdef Bran_R8
#endif

}// End of DriverTime::~DriverTime(void)

/*******************************************************************************
* Function Name  :  set_dateTime
* Description    :  设置当前时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int DriverTime::set_dateTime(QString time)
{
    qDebug()<<"SET DATE TIME";
    QProcess::execute("date");
    QString strTime = QString("date \" %1\"").arg(time);
    qDebug() << strTime;
    int res = system(strTime.toAscii());
    QProcess::execute("date");


    //打印结果
    qDebug() <<  res;
}

/*******************************************************************************
*                           陆超@2016-10-28
* Function Name :   setTimezone
* Description   :   设置时区
* Input         :   har *Timezone  时区 如"GMT-8"
* Output        :   None
* Return        :   成功返回0 失败-1
*******************************************************************************/
int DriverTime::set_timeZone(QString gmt)
{

#ifdef Bran_R8
    qDebug()<<"gmt"<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<gmt;
    int fd = -1;
    int len = 0, writeLen;
    char buffer[16];
    char *timeZone;
    if(gmt.contains("+"))
    {
        gmt.replace("+","-");
    }
    else if(gmt.contains("-"))
    {
        gmt.replace("-","+");
    }

    QByteArray ba  = gmt.toLatin1();
    timeZone = ba.data();
    QProcess::execute("date");

    // 删除原有文件
    QProcess::execute("rm -r /etc/TZ");

    fd = open("/etc/TZ", O_RDWR | O_CREAT,0777);
    if(fd < 0)
    {
        qDebug("文件打开或者创建失败！\r\n");
        return -1;
    }
    writeLen = sprintf(buffer, "%s\n", timeZone);
    len      = write(fd, buffer, writeLen);
    if(len == writeLen)
    {

        close(fd);
        QProcess::execute("date");
        qDebug("setTimezone success !\r\n");
        return 0;

    }
    else
    {
        close(fd);
        qDebug("setTimezone success faild！\r\n");
        return -1;
    }
#endif

}// End of int DriverTime::setTimezone(char *Timezone)

/*******************************************************************************
* Function Name :   sync_rtc
* Description   :   同步RTC
* Input         :   None
* Output        :   None
* Return        :   当前时间
*******************************************************************************/
int DriverTime::sync_rtc()
{
    int res;
    res = system("hwclock -s");
    qDebug() <<  "exec hwclock -s :" << res << endl;
    return res;
}

/*******************************************************************************
* Function Name  :  set_rtc
* Description    :  设置RTC
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int DriverTime::set_rtc()
{
    int res;
    res = system("hwclock -w");
    qDebug() <<  "exec hwclock -w :" << res << endl;
    return res;
}
/********************************* END OF FILE ********************************/

