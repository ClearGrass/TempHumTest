/*******************************************************************************
* File Name          :  driverButton.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt 4.8.6
* Version            :  V1.0
* Date               :  09/20/2016
* Description        :  按键驱动
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <QDebug>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "driverButton.h"
#include "globalVariable.h"

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


DriverButton *DriverButton::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverButton* DriverButton::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverButton();
    }
    return instance;
}

/*******************************************************************************
*                           陆超@2016-09-20
* Function Name  :  DriverButton
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverButton::DriverButton()
{

    // 声明时间戳 并连接槽
    timerLongPress = new QTimer();
    timerSingleClick = new QTimer();
    timerDoubleClick = new QTimer();

    timerLongPress->setSingleShot(true);
    timerSingleClick->setSingleShot(true);
    timerDoubleClick->setSingleShot(true);

    timerLongPress->setInterval(BUTTON_LONG_THRESHOLD);
    timerDoubleClick->setInterval(BUTTON_CLICK_INTERVAL);
    timerSingleClick->setInterval(BUTTON_CLICK_THRESHOLD);

    connect(timerLongPress, SIGNAL(timeout()), this, SLOT(slot_longPress_timeout()));
    connect(timerDoubleClick, SIGNAL(timeout()), this, SLOT(slot_doubleClick_timeout()));
    connect(timerSingleClick, SIGNAL(timeout()), this, SLOT(slot_singleClick_timeout()));

    isDoubleClick = false;

#ifdef Bran_R8
    driversInit = DriversInit::getInstance();
#endif
}// End of driverButton::driverButton()

/*******************************************************************************
*                           陆超@2016-09-20
* Function Name :   ~DriverButton
* Description   :   析构函数
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
DriverButton::~DriverButton(void)
{
#ifdef Bran_R8
#endif

}// End of DriverButton::~DriverButton(void)

/*******************************************************************************
*                           陆超@2016-09-20
* Function Name :   run
* Description   :   循环线程
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
void DriverButton::run(void)
{

#ifdef Bran_R8
    int fd = -1;
    struct input_event keyEvent;
    int readResult;

    QString strFile = driversInit->get_device_button();
    QByteArray ba = strFile.toLatin1();
    char *filePath = ba.data();

    // 以只读方式打开文件成功 或者 文件有内容
    fd = open(filePath, O_RDONLY);
    if (fd < 0)
    {
        qDebug() << "待添加文件打开失败处理";
    }
    while(1)
    {
        // 读取数据
        memset(&keyEvent, 0, sizeof(struct input_event));

        readResult = read(fd, &keyEvent, sizeof(struct input_event));

        if (readResult < 0)
        {
            qDebug() << "按键读取失败！\r\n" ;
        }
        else
        {
            // 检测事件
            if(keyEvent.type == EV_KEY && keyEvent.code == KEY_POWER)
            {
                if(keyEvent.value == 1)
                {
                    // 若此时正在判定是否为双击
                    //                    if(timerDoubleClick->isActive())
                    //                    {
                    //                        // 双击事件
                    //                        emit signal_button_doubleClick();
                    //                        isDoubleClick = true;
                    //                        timerDoubleClick->stop();
                    //                    }
                    //                    else
                    //                    {
                    // 启动超时判断
                    timerLongPress->start(BUTTON_LONG_THRESHOLD);
                    timerSingleClick->start(BUTTON_CLICK_THRESHOLD);
                    //                    }
                }

                //按键松开处理
                if(keyEvent.value == 0)
                {
                    if(timerSingleClick->isActive() || timerLongPress->isActive())
                    {
                        emit signal_button_singleClick();
                    }

                    // 停止计时器
                    timerLongPress->stop();
                    timerSingleClick->stop();

                    //                    if(!isDoubleClick)
                    //                    {
                    //                        // 开始判定是否为双击
                    //                        timerDoubleClick->start();
                    //                    }
                    //                    else
                    //                    {
                    //                        isDoubleClick = false;
                    //                    }
                }
            }
        }
    }

#endif
}// End of void DriverButton::run(void)

/*******************************************************************************
*                           陆超@2016-09-23
* Function Name  :  slot_clickTimeout
* Description    :  单击超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverButton::slot_singleClick_timeout()
{
    timerSingleClick->stop();
}// End of void DriverButton::slot_clickTimeout(void)

/*******************************************************************************
* Function Name  :  slot_doubleClick_timeout
* Description    :  双击超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverButton::slot_doubleClick_timeout()
{
    timerDoubleClick->stop();

    // 判定只为单击事件
    emit signal_button_singleClick();

}

/*******************************************************************************
*                           陆超@2016-09-23
* Function Name  :  slot_longTimeout
* Description    :  单击超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverButton::slot_longPress_timeout()
{
    timerLongPress->stop();
    emit signal_button_longPress();
    // 检测到长按事件
    qDebug() << "\r\n--------------------------------------------Long_Press\r\n" ;

    // 待添加相应信号事件

}// End of void DriverButton::slot_longTimeout(void)

