#include "fileTimeZone.h"

FileTimeZone *FileTimeZone::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileTimeZone* FileTimeZone::getInstance()
{
    if(0 == instance)
    {
        instance = new FileTimeZone();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FileTimeZone
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FileTimeZone::FileTimeZone(QObject *parent) : QObject(parent)
{
    listTimeZone = new QList<TimeZone>;
    typeLanguage = LANG_EN;
}

/*******************************************************************************
* Function Name  :  get_listTimeZone
* Description    :  获取时区列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<TimeZone> *FileTimeZone::get_listTimeZone(int type)
{
    typeLanguage = type;
    listTimeZone->clear();
    QFile file(FILE_TIME_ZONE);
    if(file.open(QIODevice::ReadOnly))
    {
        //声明并初始化数据流对象
        QTextStream stream(&file);
        QString strJson = stream.readAll();
        data_analyse(strJson);
        //关闭文件
        file.close();
    }
    else
    {
        qDebug()<<"NO TIME ZONE FILE";
    }
    return listTimeZone;
}

/*******************************************************************************
* Function Name  :  data_analyse
* Description    :  数据解析
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FileTimeZone::data_analyse(QString fileData)
{
    char *everyTimeZone = NULL;
    TimeZone structTimeZone;

    QByteArray ba = fileData.toLatin1();
    char *cFileData = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cFileData);
    if(NULL == root)
    {
        return;
    }
    cJSON *data = cJSON_GetObjectItem(root,"data");
    if(data)
    {
        // 获取时区个数
        int size = cJSON_GetArraySize(data);

        for(int i = 0; i < size; ++i)
        {
            // 循环解析
            cJSON *timeZone = cJSON_GetArrayItem(data,i);
            if(timeZone)
            {
                everyTimeZone = cJSON_Print(timeZone);
                cJSON *timeZoneItem = cJSON_Parse(everyTimeZone);
                structTimeZone.name = get_validString(timeZoneItem, QString(FIELD_TIME_ZONE));
                structTimeZone.gmt = get_validString(timeZoneItem, QString(FIELD_TIME_ZONE_GMT));

                if(LANG_CN == typeLanguage)
                {
                    structTimeZone.description = get_validString(timeZoneItem, FIELD_DESCRIPTION_CN);
                }
                else if(LANG_TW == typeLanguage)
                {
                    structTimeZone.description = get_validString(timeZoneItem, FIELD_DESCRIPTION_TW);
                }
                else
                {
                    structTimeZone.description = get_validString(timeZoneItem, FIELD_DESCRIPTION_EN);
                }
                listTimeZone->push_front(structTimeZone);
            }
        }
    }

}

/*******************************************************************************
* Function Name  :  get_validString
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString FileTimeZone::get_validString(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return QString::fromUtf8(cJSON_GetObjectItem(item, cField)->valuestring);
    }
    return "";
}
