#ifndef SYSTEMLANGUAGE_H
#define SYSTEMLANGUAGE_H
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTranslator>
#include "file/config/fileConfig.h"
#include "globalVariable.h"

#define TRANSLATOR_CN   "ZH_SC.qm"
#define TRANSLATOR_TW   "ZH_TC.qm"
#define TRANSLATOR_EN   "US.qm"

class SysLanguage : public QObject
{
    Q_OBJECT
public:
    static SysLanguage* getInstance();                                  //
    LanguageType get_languageType();                                    // 获取此时系统语言版本号
    bool is_languageInit();
                                                  //当前系统语言
signals:
    void signal_change_language(LanguageType);
    void signal_device_reboot();

protected slots:
    void slot_change_language(LanguageType);
    void slot_set_language(LanguageType);

private:
    explicit SysLanguage(QObject *parent = 0);
    void data_init();
    void language_init(LanguageType);

private:
    LanguageType language;
    FileConfig *fileConfig;

    QString lang;
    bool isLanguageInit;

private:
    static SysLanguage *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysLanguage::instance)
                delete SysLanguage::instance;
        }
    };
    static FreeInstance free;
};

#endif // SYSTEMLANGUAGE_H
