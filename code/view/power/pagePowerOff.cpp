#include "pagePowerOff.h"
#include <QDebug>
PagePowerOff::PagePowerOff(QWidget *parent)
    : QDeclarativeView(parent)
{
    this->setGeometry(QRect(0,0,854,480));
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background: transparent;border:0px");
    QDeclarativeEngine *engine = this->engine();
    QDeclarativeContext *context = engine->rootContext();
    context->setContextProperty("PagePowerOff",this);
    this->setSource(QUrl(QML_POWER_OR_SCREEN_OFF_URL));

}

/*******************************************************************************
* Function Name  :  show_page
* Description    :  显示页面
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void PagePowerOff::show_page()
{
    this->show();
    emit signal_startTimer();
}

/*******************************************************************************
* Function Name :   slot_power_off
* Description   :   关机
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PagePowerOff::slot_power_off()
{
    emit signal_device_powerOff();
    this->hide();
}

/*******************************************************************************
* Function Name  :  slot_power_on
* Description    :  进入系统
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void PagePowerOff::slot_cancel()
{
    this->hide();
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-10
* Function Name :   slot_changeHorizonScreen
* Description   :   切换横屏
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PagePowerOff::slot_changeHorizonScreen()
{
    qDebug() << "slot_changeHorizonScreen";
    emit signal_horrizonScreen();
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-10
* Function Name :   slot_changevertialScreen
* Description   :   切换竖屏
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PagePowerOff::slot_changevertialScreen()
{
    qDebug() << "slot_changevertialScreen";
    emit signal_vertialScreen();
}

/*******************************************************************************
* Function Name  :  slot_screen_off
* Description    :  关闭屏幕
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void PagePowerOff::slot_screen_off()
{
    emit signal_device_screenOff();
    this->hide();
}
