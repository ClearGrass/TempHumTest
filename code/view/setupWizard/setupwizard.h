#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QDialog>
#include <QTextCodec>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <qdeclarative.h>
#include "file/config/fileConfig.h"
#include "pageSetting.h"
#include "sysLanguage.h"

#define QML_WIZARDURL "qrc:/resource/qml/SetupWizard/LanguageWizard.qml"

namespace Ui {
class SetupWizard;
}

class SetupWizard : public QDialog
{
    Q_OBJECT
public:
    explicit SetupWizard(QWidget *parent = 0);
    QString get_language();
    QString get_curLanguage();
    ~SetupWizard();

signals:
    void signal_wizard_finish();
    void signal_set_language(LanguageType);

public slots:
    void slot_setLanguage(int index);
    void slot_jumplanWizard();
    void slot_wizardFinish();

private:
    Ui::SetupWizard *ui;
    QString strLanguage;
    QString curLanguage;

    LanguageType language;

    FileConfig  *fileConfig;
    PageSetting *pageSetting;
    SysLanguage *sysLanguage;
    QDeclarativeView *qmlview;

};

#endif // SETUPWIZARD_H
