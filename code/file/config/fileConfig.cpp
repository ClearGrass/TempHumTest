#include "fileConfig.h"

FileConfig *FileConfig::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileConfig* FileConfig::getInstance()
{
    if(0 == instance)
    {
        instance = new FileConfig();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FileConfig
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileConfig::FileConfig()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    list_init();
    config_init();

}

/*******************************************************************************
* Function Name  :  config_init
* Description    :  配置初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::config_init()
{
    QFile file(PATH_BRAN_CONF);
    if(file.open(QIODevice::ReadOnly))
    {
        //设置字符编码
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        //声明并初始化数据流对象
        QTextStream stream(&file);
        QString strReadLine;
        //读取文件内容
        while ( !stream.atEnd())
        {
            strReadLine = stream.readLine();
            QStringList list = strReadLine.split("=");
            if(list.size() == 2)
            {
                QMutableListIterator<SystemValue> i(globalList);
                while(i.hasNext())
                {
                    if(i.next().name == strReadLine.split("=").at(0).trimmed())
                    {
                        SystemValue g(strReadLine.split("=").at(0).trimmed(),strReadLine.split("=").at(1).trimmed());
                        i.setValue(g);
                    }
                }
            }
        }
        //关闭文件
        file.close();
    }
    else
    {
        qDebug()<<"NO CONFIG FILE";
    }
    if(this->get_value(DEVICE_SN) == DEFAULT_DEVICE_SN)
    {
        sn = get_device_sn();

        // 判断sn是否为合理数值
        this->set_value(DEVICE_SN,sn);
    }
    if(this->get_value(DEVICE_MAC) == DEFAULT_DEVICE_MAC)
    {
        mac = get_device_mac();

        // 判断mac是否为合理数值
        this->set_value(DEVICE_MAC,mac);
    }
    if(this->get_value(MIIO_DID) == DEFAULT_MIIO_DID)
    {
        this->set_value(MIIO_DID, get_miio_did());
    }
    if(this->get_value(MIIO_MAC) == DEFAULT_MIIO_MAC)
    {
        this->set_value(MIIO_MAC, get_miio_mac());
    }
    if(this->get_value(MIIO_KEY) == DEFAULT_MIIO_KEY)
    {
        this->set_value(MIIO_KEY, get_miio_key());
    }

}


/*******************************************************************************
*                           陆超@2016-11-15
* Function Name  :  list_init
* Description    :  初始化全局配置
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::list_init(void)
{
    // 初始化全局配置列表
    globalList.append(SystemValue(DEVICE_INIT,      DEFAULT_DEVICE_INIT));

    globalList.append(SystemValue(TEMP_UNIT,        DEFAULT_TEMP_UNIT));
    globalList.append(SystemValue(TVOC_UNIT,        DEFAULT_TVOC_UNIT));

    globalList.append(SystemValue(CITY_AUTO,        DEFAULT_CITY_AUTO));
    globalList.append(SystemValue(CITY_ID,          DEFAULT_CITY_ID));
    globalList.append(SystemValue(CITY_NAME_EN,     DEFAULT_CITY_NAME_EN));
    globalList.append(SystemValue(CITY_NAME_SC,     DEFAULT_CITY_NAME_SC));
    globalList.append(SystemValue(CITY_NAME_TC,     DEFAULT_CITY_NAME_TC));
    globalList.append(SystemValue(SUNRISE_TIME,     DEFAULT_SUNRISE_TIME));
    globalList.append(SystemValue(SUNSET_TIME,      DEFAULT_SUNSET_TIME));
    globalList.append(SystemValue(SUN_INIT,         DEFAULT_SUN_INIT));

    globalList.append(SystemValue(DEVICE_ID,        DEFAULT_DEVICE_ID));
    globalList.append(SystemValue(DEVICE_SN,        DEFAULT_DEVICE_SN));
    globalList.append(SystemValue(DEVICE_MAC,       DEFAULT_DEVICE_MAC));
    globalList.append(SystemValue(DEVICE_MODEL,     DEFAULT_DEVICE_MODEL));

    globalList.append(SystemValue(MIIO_DID,         DEFAULT_MIIO_DID));
    globalList.append(SystemValue(MIIO_MAC,         DEFAULT_MIIO_MAC));
    globalList.append(SystemValue(MIIO_KEY,         DEFAULT_MIIO_KEY));

    globalList.append(SystemValue(TOKEN,            DEFAULT_TOKEN));
    globalList.append(SystemValue(EXP_TIME,         DEFAULT_EXP_TIME));

    globalList.append(SystemValue(LIGHT_AUTO,       DEFAULT_LIGHT_AUTO));
    globalList.append(SystemValue(LIGHT_VALUE,      DEFAULT_LIGHT_VALUE));

    globalList.append(SystemValue(TIME_INIT,        DEFAULT_TIME_INIT));
    globalList.append(SystemValue(TIME_AUTO,        DEFAULT_TIME_AUTO));
    globalList.append(SystemValue(TIME_MODE,        DEFAULT_TIME_MODE));
    globalList.append(SystemValue(TIME_ZONE,        DEFAULT_TIME_ZONE));

    globalList.append(SystemValue(LANGUAGE_INIT,    DEFAULT_LANGUAGE_INIT));
    globalList.append(SystemValue(LANGUAGE,         DEFAULT_LANGUAGE));

    globalList.append(SystemValue(NIGHT_MODE,       DEFAULT_NIGHT_MODE));
    globalList.append(SystemValue(NIGHT_AUTO,       DEFAULT_NIGHT_AUTO));
    globalList.append(SystemValue(NIGHT_START_TIME, DEFAULT_NIGHT_START_TIME));
    globalList.append(SystemValue(NIGHT_END_TIME,   DEFAULT_NIGHT_END_TIME));

    globalList.append(SystemValue(WIFI_MAIN_PAGE,   DEFAULT_WIFI_MAIN_PAGE));
    globalList.append(SystemValue(SYS_ENTER,        DEFAULT_SYS_ENTER));
    globalList.append(SystemValue(SYS_INIT,         DEFAULT_SYS_INIT));

    globalList.append(SystemValue(APP_BIND_STATUS,  DEFAULT_APP_BIND_STATUS));
    globalList.append(SystemValue(SOFTWARE_UPGRADE, DEFAULT_SOFTWARE_UPGRADE));

    globalList.append(SystemValue(PM25_SAMPLING_CHARGING,       DEFAULT_PM25_SAMPLING_CHARGING));
    globalList.append(SystemValue(PM25_SAMPLING_DISCHARGING,    DEFAULT_PM25_SAMPLING_DISCHARGING));
    globalList.append(SystemValue(SCREEN_OFF_CHARGING,          DEFAULT_SCREEN_OFF_CHARGING));
    globalList.append(SystemValue(SCREEN_OFF_DISCHARGING,       DEFAULT_SCREEN_OFF_DISCHARGING));

    globalList.append(SystemValue(DOUBLE_CLICK_CHECKING,       DEFAULT_DOUBLE_CLICK_CHECKING));

    globalList.append(SystemValue(VERTICAL_SCREEN_REFRESH,       DEFAULT_VERTICAL_SCREEN_REFRESH));


}// End of void SysConfig::list_init(void)

/*******************************************************************************
* Function Name  :  config_save
* Description    :  保存设置
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::config_save()
{
    QFile file(PATH_BRAN_CONF);
    if(file.open(QFile::WriteOnly))
    {
        //声明并初始化数据流对象
        QTextStream stream(&file);
        stream.setCodec("utf-8");
        foreach(SystemValue object,globalList)
        {
            QString str = object.name + " = " + object.value;
            stream<<str<<"\n";
        }
        file.close();
    }
}

/*******************************************************************************
* Function Name  :  get_value
* Description    :  获取指定值
* Input          :  QString name    指定值
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_value(QString name)
{
    if(globalList.isEmpty())
    {
        config_init();
    }
    foreach(SystemValue object,globalList)
    {
        if(name == object.name)
        {
            return object.value;
        }
    }
    return NULL;
}

/*******************************************************************************
* Function Name  :  set_value
* Description    :  设置固定值
* Input          :  QString name    字段名
*                   QString value   值
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileConfig::set_value(QString name, QString value)
{
    if(globalList.isEmpty())
    {
        config_init();
    }
    QMutableListIterator<SystemValue> i(globalList);

    while(i.hasNext())
    {
        if(i.next().name == name)
        {
            SystemValue g(name,value);
            i.setValue(g);
        }
    }
    config_save();
}

/*******************************************************************************
* Function Name  :  get_device_sn
* Description    :  获取序列号
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_device_sn()
{
#ifdef Bran_R8
    // 获取本机设备序列号
    QString sn;
    QFile file(FILE_CPU);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly))
    {
        //设置编码格式
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);
        QString strReadLine = stream.readLine();
        while(!strReadLine.isNull())
        {
            if(strReadLine.split(":").at(0).trimmed() == "Chipid")
            {
                sn = strReadLine.split(":").at(1).trimmed();
                qDebug()<<sn;
                file.close();
                return sn;
            }
            strReadLine = stream.readLine();
        }
        file.close();
    }
    else
    {
        qDebug()<< file.errorString();
    }
    return sn;
#else
    return "testPC";
#endif
}

/*******************************************************************************
* Function Name  :  get_miio_did
* Description    :  获取设备did
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_miio_did()
{
    QFile file("/bin/qtapp/device.conf");
    if(!file.exists())
    {
        return "";
    }
    FILE  *stream;
    char  buf[4096];
    QString result;
    memset(buf, 0, sizeof(buf));
    stream = popen("cat /bin/qtapp/device.conf | grep -v ^# | grep did= | tail -1 | cut -d '=' -f 2", "r");
    fread( buf, sizeof(char), sizeof(buf),  stream);
    result = buf;
    pclose(stream);
    return result.trimmed();
}

/*******************************************************************************
* Function Name  :  get_miio_mac
* Description    :  获取mac
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_miio_mac()
{
    QFile file("/bin/qtapp/device.conf");
    if(!file.exists())
    {
        return "";
    }
    FILE  *stream;
    char  buf[4096];
    QString result;
    memset(buf, 0, sizeof(buf));
    stream = popen("cat /bin/qtapp/device.conf | grep -v ^# | grep mac= | tail -1 | cut -d '=' -f 2", "r");
    fread( buf, sizeof(char), sizeof(buf),  stream);
    result = buf;
    pclose(stream);
    return result.trimmed();
}

/*******************************************************************************
* Function Name  :  get_miio_key
* Description    :  获取key
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_miio_key()
{
    QFile file("/bin/qtapp/device.conf");
    if(!file.exists())
    {
        return "";
    }
    FILE  *stream;
    char  buf[4096];
    QString result;
    memset(buf, 0, sizeof(buf));
    stream = popen("cat /bin/qtapp/device.conf | grep -v ^# | grep key= | tail -1 | cut -d '=' -f 2", "r");
    fread( buf, sizeof(char), sizeof(buf),  stream);
    result = buf;
    pclose(stream);
    return result.trimmed();
}

/*******************************************************************************
* Function Name  :  get_device_mac
* Description    :  获取设备mac地址
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FileConfig::get_device_mac()
{
    QString mac;
    //获取所有网络接口的列表
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        //MAC地址
        mac = netInterface.hardwareAddress();
    }
    return mac;
}


/*******************************************************************************
* Function Name :   config_reset
* Description   :   恢复出厂设置
* Input         :   None
* Output        :   初始化
* Return        :   None
*******************************************************************************/
void FileConfig::config_reset()
{
    globalList.clear();
    list_init();
    config_save();
}

