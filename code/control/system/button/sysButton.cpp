/*******************************************************************************
* File Name          :  sysButton.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt 4.8.6
* Version            :  V1.0
* Date               :  09/20/2016
* Description        :  主循环程序
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <QDebug>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "sysButton.h"
#include "globalVariable.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

SysButton *SysButton::instance = NULL;

SysButton *SysButton::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysButton();
    }
    return instance;
}

/*******************************************************************************
*                           陆超@2016-09-20
* Function Name  :  SysButton
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysButton::SysButton()
{
    driverButton = DriverButton::getInstance();
    connect(driverButton, SIGNAL(signal_button_singleClick()), this, SLOT(slot_button_singleClick()));
    connect(driverButton, SIGNAL(signal_button_doubleClick()), this, SLOT(slot_button_doubleClick()));
    connect(driverButton, SIGNAL(signal_button_longPress()), this, SLOT(slot_button_longPress()));
    driverButton->start();
    moveToThread(this);
}// End of SysButton::SysButton()

/*******************************************************************************
*                           陆超@2016-09-20
* Function Name :   ~SysButton
* Description   :   析构函数
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
SysButton::~SysButton(void)
{
#ifdef Bran_R8
#endif
}// End of SysButton::~SysButton(void)

/*******************************************************************************
* Function Name  :  slot_button_singleClick
* Description    :  单击事件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysButton::slot_button_singleClick()
{
    qDebug()<<"#################################################################BUTTON_SINGLE_CLICK";
    emit signal_button_singleClick();
}

/*******************************************************************************
* Function Name  :  slot_button_doubleClick
* Description    :  双击事件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysButton::slot_button_doubleClick()
{
    qDebug()<<"#################################################################BUTTON_DOUBLE_CLICK";
    emit signal_button_doubleClick();
}

/*******************************************************************************
* Function Name  :  slot_button_longPress
* Description    :  长按事件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysButton::slot_button_longPress()
{
    qDebug()<<"#################################################################BUTTON_LONG_PRESS";
    emit signal_button_longPress();
}

/********************************* END OF FILE ********************************/
