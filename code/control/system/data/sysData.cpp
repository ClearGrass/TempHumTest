#include "sysData.h"

SysData *SysData::instance = NULL;

/*******************************************************************************
* Function Name :   getInstance
* Description   :   获取系统时间类 单例实例
* Input         :   None
* Output        :   None
* Return        :   返回单例 对象实例
*******************************************************************************/
SysData *SysData::getInstance(void)
{
    // 是否被创建过
    if(NULL == instance)
    {
        instance = new SysData();
    }
    return instance;

}// End of SysTime* SysTime::getInstance()

/*******************************************************************************
* Function Name  :  SysData
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysData::SysData(QObject *parent)
    : QThread()
{
    qRegisterMetaType<UnitType>("UnitType");
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
void SysData::data_init()
{
    fileConfig = FileConfig::getInstance();
    QString strTVOC = fileConfig->get_value(TVOC_UNIT);
    QString strTEMP = fileConfig->get_value(TEMP_UNIT);
    QString strIntervalCharging = fileConfig->get_value(PM25_SAMPLING_CHARGING);
    QString strIntervalDischarging = fileConfig->get_value(PM25_SAMPLING_DISCHARGING);

    // 获取配置的单位
    if(strTVOC == "ppm")
    {
        unitTVOC = PPM;
    }
    else if(strTVOC == "ppb")
    {
        unitTVOC = PPB;
    }
    else
    {
        unitTVOC = MG_M3;
    }

    // 获取配置的单位
    if(strTEMP == "kfc")
    {
        unitTEMP = KFC;
    }
    else
    {
        unitTEMP = CEL;
    }

    if(strIntervalCharging == "5")
    {
        intervalCharging = MIN5;
    }
    else if(strIntervalCharging == "30")
    {
        intervalCharging = MIN30;
    }
    else
    {
        intervalCharging = MIN15;
    }

    if(strIntervalDischarging == "5")
    {
        intervalDischarging = MIN5;
    }
    else if(strIntervalDischarging == "15")
    {
        intervalDischarging = MIN15;
    }
    else
    {
        intervalDischarging = MIN30;
    }

}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::connect_init()
{

}

/*******************************************************************************
* Function Name  :  get_unit
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
UnitType SysData::get_unitTEMP()
{
    return unitTEMP;
}

/*******************************************************************************
* Function Name  :  get_unitTVOC
* Description    :  获取TVOC此时单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
UnitType SysData::get_unitTVOC()
{
    return unitTVOC;
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
IntervalType SysData::get_intervalCharging()
{
    return intervalCharging;
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
IntervalType SysData::get_intervalDischarging()
{
    return intervalDischarging;
}

/*******************************************************************************
* Function Name  :  slot_set_unitTEMP
* Description    :  设置温度单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::slot_change_unitTEMP(UnitType unit)
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    unitTEMP = unit;

    emit signal_change_unitTEMP(unit);

    // 设置温度的单位
    switch (unit)
    {
    case KFC:
        qDebug()<<"@@@@@@@@@@@@@@@@@";
        fileConfig->set_value(TEMP_UNIT, "kfc");
        break;
    default:
        fileConfig->set_value(TEMP_UNIT, "cel");
        break;
    }
}

/*******************************************************************************
* Function Name  :  slot_set_unitTVOC
* Description    :  设置TVOC单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::slot_change_unitTVOC(UnitType unit)
{
    unitTVOC = unit;

    emit signal_change_unitTVOC(unit);

    // 设置tvoc的单位
    switch (unit)
    {
    case PPM:
        fileConfig->set_value(TVOC_UNIT, "ppm");
        break;
    case PPB:
        fileConfig->set_value(TVOC_UNIT, "ppb");
        break;
    default:
        fileConfig->set_value(TVOC_UNIT, "mg/m3");
        break;
    }
}

/*******************************************************************************
* Function Name  :  slot_intervalSampling_charging
* Description    :  改变PM2.5检索频率
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::slot_intervalSampling_charging(IntervalType interval)
{
    if(interval != intervalCharging)
    {
        switch(interval)
        {
        case MIN5:
            fileConfig->set_value(PM25_SAMPLING_CHARGING, "5");
            break;
        case MIN15:
            fileConfig->set_value(PM25_SAMPLING_CHARGING, "15");
            break;
        case MIN30:
            fileConfig->set_value(PM25_SAMPLING_CHARGING, "30");
            break;
        default:
            return;
            break;
        }
        intervalCharging = interval;
        emit signal_intervalSampling_charging(interval);
    }
}

/*******************************************************************************
* Function Name  :  slot_intervalSampling_discharging
* Description    :  改变PM2.5检测频率
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysData::slot_intervalSampling_discharging(IntervalType interval)
{
    if(interval != intervalDischarging)
    {
        switch(interval)
        {
        case MIN5:
            fileConfig->set_value(PM25_SAMPLING_DISCHARGING, "5");
            break;
        case MIN15:
            fileConfig->set_value(PM25_SAMPLING_DISCHARGING, "15");
            break;
        case MIN30:
            fileConfig->set_value(PM25_SAMPLING_DISCHARGING, "30");
            break;
        default:
            return;
            break;
        }
        intervalDischarging = interval;
        emit signal_intervalSampling_discharging(interval);
    }
}

