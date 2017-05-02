#include "pagePowerOn.h"

PagePowerOn::PagePowerOn(QWidget *parent)
    : QDeclarativeView(parent)
{
    this->setSource(QUrl(QML_POWERON_SOURCE_URL));
    QDeclarativeEngine *engine = this->engine();
    QDeclarativeContext *context = engine->rootContext();
    context->setContextProperty("PagePowerOn",this);
}

/*******************************************************************************
* Function Name :   slot_power_off
* Description   :   关机
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PagePowerOn::slot_power_off()
{
    this->hide();
    this->deleteLater();
    emit signal_device_powerOff();
}

/*******************************************************************************
* Function Name  :  slot_power_on
* Description    :  进入系统
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void PagePowerOn::slot_power_on()
{
    this->hide();
    this->deleteLater();
    emit signal_system_start();
}
