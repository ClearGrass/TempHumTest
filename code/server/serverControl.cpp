#include "serverControl.h"

ServerControl *ServerControl::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerControl *ServerControl::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  ServerControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerControl::ServerControl(QObject *parent)
    :QThread(parent)
{
    database  = Database::getInstance();
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
void ServerControl::data_init()
{
    sysDevice      = SysDevice::getInstance();

    listDataDaily  = new QList<AirDataDaily>;
    listDataHourly = new QList<AirDataHourly>;
    serverAirData  = ServerAirData::getInstance();
    serverCity     = ServerCity::getInstance();
    serverDevice   = ServerDevice::getInstance();
    serverFirmware = ServerFirmware::getInstance();
    serverWeather  = ServerWeather::getInstance();
    serverDebug    = ServerDebug::getInstance();

    serverAirData->moveToThread(this);
    serverDevice->moveToThread(this);
    serverCity->moveToThread(this);
    serverFirmware->moveToThread(this);
    serverWeather->moveToThread(this);
    serverDebug->moveToThread(this);

    timerPush     = new QTimer();
    timerPush->setInterval(60*60*1000);

    // 获取数据库缓存表的数据
    listDataDaily = database->get_cacheDat_daily();
    listDataHourly = database->get_cacheData_hourly();

    isDeviceInit = sysDevice->is_deviceInit();
    isConnected = false;
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerControl::connect_init()
{
    connect(timerPush, SIGNAL(timeout()), this, SLOT(slot_push_data()));
    connect(this, SIGNAL(signal_push_data(QString,QString,QString,QString)), serverAirData, SLOT(slot_push_data(QString,QString,QString,QString)));
}

/*******************************************************************************
* Function Name  :  slot_push_data
* Description    :  推送空气数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerControl::slot_push_data()
{
    int i;
    char *cDatas = NULL;
    char *cDaily = NULL;
    cJSON *datas = NULL;
    cJSON *daily = NULL;
    cJSON *data  = NULL;

    // 建立新的Json数据包
    if(!listDataHourly->isEmpty())
    {
        datas = cJSON_CreateArray();

        if(NULL == datas)
        {
            return;
        }

        for(i = 0; i < listDataHourly->length(); ++i)
        {
            data = cJSON_CreateObject();
            switch(listDataHourly->at(i).type)
            {
            case PM25:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("pm25"));
                break;
            }
            case TEMP:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("temperature"));
                break;
            }
            case HUMI:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("humidity"));
                break;
            }
            case TVOC:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("tvoc"));
                break;
            }
            case CO2E:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("co2"));
                break;
            }
            default:
                break;
            }
            cJSON_AddItemToObject(data, "value", cJSON_CreateNumber(listDataHourly->at(i).value));
            cJSON_AddItemToObject(data, "timestamp", cJSON_CreateNumber(listDataHourly->at(i).timestamp));

            cJSON_AddItemToArray(datas, data);
        }
    }

    if(!listDataDaily->isEmpty())
    {
        daily = cJSON_CreateArray();
        if(NULL == daily)
        {
            return;
        }
        for(i = 0; i < listDataDaily->length(); ++i)
        {
            data = cJSON_CreateObject();
            switch(listDataDaily->at(i).type)
            {
            case PM25:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("pm25"));
                break;
            }
            case TEMP:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("temperature"));
                break;
            }
            case HUMI:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("humidity"));
                break;
            }
            case TVOC:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("tvoc"));
                break;
            }
            case CO2E:
            {
                cJSON_AddItemToObject(data, "type", cJSON_CreateString("co2"));
                break;
            }
            default:
                break;
            }
            cJSON_AddItemToObject(data, "max", cJSON_CreateNumber(listDataDaily->at(i).max));
            cJSON_AddItemToObject(data, "min", cJSON_CreateNumber(listDataDaily->at(i).min));
            cJSON_AddItemToObject(data, "timestamp", cJSON_CreateNumber(listDataDaily->at(i).timestamp));
            cJSON_AddItemToArray(daily, data);
        }
    }
    cDatas = cJSON_Print(datas);
    cDaily = cJSON_Print(daily);

    if((!listDataDaily->isEmpty()|| !listDataHourly->isEmpty())&& isConnected && isDeviceInit)
    {
        emit signal_push_data(deviceID, QString(cDatas), QString(cDaily), "");
    }
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerControl::slot_net_connected()
{
    isConnected = true;
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerControl::slot_net_disconnect()
{
    isConnected = false;
}


void ServerControl::slot_update_deviceID(QString deviceID)
{
    isDeviceInit = true;
    this->deviceID = deviceID;
}
