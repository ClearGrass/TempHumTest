#include "dropDown.h"

DropDown::DropDown(QWidget *parent)
    : QDeclarativeView(parent)
{
    this->setStyleSheet("background-color:transparent");
    QDeclarativeEngine *engine = this->engine();
    QDeclarativeContext *context = engine->rootContext();
    context->setContextProperty("DropDown",this);
    sysControl = SysControl::getInstance();
    isNight = sysControl->get_night();
    sysTime    = SysTime::getInstance();

    connect(this, SIGNAL(signal_set_night(bool)), sysTime, SLOT(slot_set_night(bool)));
    connect(this, SIGNAL(signal_set_lightMode(bool)), sysControl, SLOT(slot_set_lightMode(bool)));
    connect(this, SIGNAL(signal_set_lightValue(int)), sysControl, SLOT(slot_set_lightValue(int)));
    connect(sysControl, SIGNAL(signal_light_value(int)),this,SLOT(slot_auto_changeValue(int)));
    connect(sysControl, SIGNAL(signal_in_night(bool)), this, SLOT(slot_in_night(bool)));

    this->setSource(QUrl(QML_DROPDOWN_SOURCE_URL));


}

/*******************************************************************************
* Function Name  :  slot_screen_off
* Description    :  关闭屏幕
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DropDown::slot_screen_off()
{
    emit signal_device_screenOff();
    emit signal_dropUp();
}

bool DropDown::slot_getNightMode()
{
    return isNight;
}

void DropDown::slot_setNightMode(bool mode)
{
    isNight = mode;
    emit signal_set_night(mode);
    emit signal_dropUp();
}

bool DropDown::slot_isAuto()
{
    if(sysControl->get_value(LIGHT_AUTO) == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DropDown::slot_setAutoChangeLight(bool checked)
{
    emit signal_set_lightMode(checked);

    if(checked)
    {
        sysControl->set_value(LIGHT_AUTO,"true");
    }
    else
    {
        sysControl->set_value(LIGHT_AUTO,"false");
    }
}

void DropDown::slot_setLightValue(int value)
{
    // 设置滑块滑动的最小值
    if (value <= 0)
    {
        value = 0;
    }
    else if (value >= 100)
    {
        value = 100;
    }
    sysControl->set_value(LIGHT_VALUE,QString::number(value));

    emit signal_set_lightValue(value);
}

void DropDown::slot_auto_changeValue(int value)
{
    curLightValue = value;
    emit signal_autoChangeValue();
}


int DropDown::slot_getLightValue()
{
    return curLightValue;
}

void DropDown::slot_in_night(bool isNight)
{
    this->isNight = isNight;
    emit signal_updateNightMode();
}
