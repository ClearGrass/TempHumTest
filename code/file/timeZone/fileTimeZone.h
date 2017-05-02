#ifndef FILETIMEZONE_H
#define FILETIMEZONE_H
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include "3rdparty/cJSON/cJSON.h"
#include "globalStruct.h"

#define FILE_TIME_ZONE          ":/resource/files/timeZone.json"
#define FIELD_TIME_ZONE         "timeZone"
#define FIELD_TIME_ZONE_GMT     "timeZone_gmt"
#define FIELD_DESCRIPTION_EN    "description_EN"
#define FIELD_DESCRIPTION_CN    "description_CN"
#define FIELD_DESCRIPTION_TW    "description_TW"

class FileTimeZone : public QObject
{
    Q_OBJECT
public:
    static FileTimeZone *getInstance();
    QList<TimeZone> *get_listTimeZone(int type);

private:
    explicit FileTimeZone(QObject *parent = 0);

private:
    void data_analyse(QString);
    QString get_validString(cJSON *item, QString field);

private:
    QList<TimeZone> *listTimeZone;
    int typeLanguage;

private:
    static FileTimeZone *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FileTimeZone::instance)
                delete FileTimeZone::instance;
        }
    };
    static FreeInstance free;
};

#endif // FILETIMEZONE_H
