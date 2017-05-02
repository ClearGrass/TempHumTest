#ifndef FILECONFIG_H
#define FILECONFIG_H

#include <QList>
#include <QStringList>
#include <QObject>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include <QProcess>
#include <QNetworkInterface>
#include "globalVariable.h"
#include "globalDefine.h"
#include "globalConfigDefine.h"

#define     FILE_CPU                "/proc/cpuinfo"                     // CPU信息文档

typedef struct SystemValue
{
    QString name;
    QString value;

    SystemValue(QString name,QString value)
    {
        this->name = name;
        this->value = value;
    }

}SystemValue;
class FileConfig : public QObject
{
    Q_OBJECT
public:
    static FileConfig *getInstance();
    QString get_value(QString);
    void set_value(QString name,QString value);
    void config_reset();

protected:
    FileConfig();
    void list_init(void);                                               // 初始化全局配置
    void config_init();
    void config_save();

    QString get_device_sn();
    QString get_device_mac();

    QString get_miio_did();
    QString get_miio_mac();
    QString get_miio_key();

private:
    QList<SystemValue> globalList;

    QString sn;
    QString mac;
    QString model;

private:
    static FileConfig *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FileConfig::instance)
                delete FileConfig::instance;
        }
    };
    static FreeInstance free;

};

#endif // FILECONFIG_H
