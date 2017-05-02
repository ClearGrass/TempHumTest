#include "setupwizard.h"
#include "ui_setupwizard.h"

SetupWizard::SetupWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupWizard)
{
    ui->setupUi(this);
    fileConfig  = FileConfig::getInstance();
    sysLanguage = SysLanguage::getInstance();

    language = sysLanguage->get_languageType();
    strLanguage = fileConfig->get_value(LANGUAGE);
    curLanguage = fileConfig->get_value(LANGUAGE);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // 支持中文路径
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); //支持中文文件名显示

    qmlview  = new QDeclarativeView(ui->page_lan);
    qmlRegisterType<SetupWizard>("SetupWizard", 1, 0, "SetupWizard");
    qmlview->engine()->rootContext()->setContextProperty("SetupWizardProperty", this);
    qmlview->setSource(QUrl(QML_WIZARDURL));
    qmlview->show();
    pageSetting = new PageSetting(ui->page_wifi);
    pageSetting->slot_setQMl(15);
    ui->stackedWidget->setCurrentIndex(0);
    connect(pageSetting, SIGNAL(signal_jumpLangeWizard()), this, SLOT(slot_jumplanWizard()));
    connect(pageSetting, SIGNAL(signal_wizardFinish()), this, SLOT(slot_wizardFinish()));
    connect(this, SIGNAL(signal_set_language(LanguageType)), sysLanguage, SLOT(slot_set_language(LanguageType)));
}

QString SetupWizard::get_language()
{
    return strLanguage;
}

QString SetupWizard::get_curLanguage()
{
    return curLanguage;
}

SetupWizard::~SetupWizard()
{
    delete ui;
}

/*******************************************************************************
* Author        :   虎正玺@2016-12-14
* Function Name :   slot_setLanguage
* Description   :   设置多语言文件
* Input         :   int index：语言下标标记
* Output        :   None
* Return        :   None
*******************************************************************************/
void SetupWizard::slot_setLanguage(int index)
{
    switch (index) {
    case 0: //中文简体
        language = LANG_CN;
        strLanguage = TRANSLATOR_CN;
        break;
    case 1://中文繁体
        language = LANG_TW;
        strLanguage = TRANSLATOR_TW;
        break;
    case 2://英文
        language = LANG_EN;
        strLanguage = TRANSLATOR_EN;
        break;
    default:
        language = LANG_EN;
        strLanguage = TRANSLATOR_EN;
        break;
    }
    //设置当前页面
    ui->stackedWidget->setCurrentIndex(1);

    //修改wifi页面语言
    pageSetting->set_wifiWizardLanguage(strLanguage);

    //设置wifi页面入口类型为设置向导
    pageSetting->set_wifiEntery(3);
}

void SetupWizard::slot_jumplanWizard()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void SetupWizard::slot_wizardFinish()
{
    this->hide();
    this->deleteLater();
    emit signal_set_language(language);
    emit signal_wizard_finish();

}
