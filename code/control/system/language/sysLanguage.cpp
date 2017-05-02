#include "sysLanguage.h"

SysLanguage *SysLanguage::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例个体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysLanguage* SysLanguage::getInstance()
{
    if(0 == instance)
    {
        instance = new SysLanguage();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysLanguage
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysLanguage::SysLanguage(QObject *parent)
    : QObject(parent)
{
    data_init();
}

/*******************************************************************************
* Function Name  :  is_languageInit
* Description    :  语言是否初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysLanguage:: is_languageInit()
{
    if("true" == fileConfig->get_value(LANGUAGE_INIT))
    {
        return true;
    }
    if("true" == fileConfig->get_value(DEVICE_INIT))
    {
        fileConfig->set_value(LANGUAGE_INIT, "true");
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLanguage::data_init()
{
    fileConfig = FileConfig::getInstance();
    lang = fileConfig->get_value(LANGUAGE);

    // 默认语言已初始化
    isLanguageInit = true;

    if("false" == fileConfig->get_value(LANGUAGE_INIT))
    {
        if("true" == fileConfig->get_value(DEVICE_INIT))
        {
            fileConfig->set_value(LANGUAGE_INIT, "true");
        }
        else
        {
            isLanguageInit = false;
        }
    }

    if(TRANSLATOR_CN == lang)
    {
        language = LANG_CN;
    }
    else if(TRANSLATOR_TW == lang)
    {
        language = LANG_TW;
    }
    else if(TRANSLATOR_EN)
    {
        language = LANG_EN;
    }
    else
    {
        isLanguageInit = false;
        language = LANG_EN;
    }

    // 语言已被初始化
    if(isLanguageInit)
    {
        language_init(language);
    }
}

/*******************************************************************************
* Function Name  :  slot_change_language
* Description    :  改变语言
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLanguage::slot_change_language(LanguageType type)
{
    // 若此时所选语言类型和系统一致
    if((language == type) && isLanguageInit)
    {
        return;
    }

    language = type;

    if(type == LANG_CN)
    {
        fileConfig->set_value(LANGUAGE, "ZH_SC.qm");
    }
    else if(type == LANG_TW)
    {
        fileConfig->set_value(LANGUAGE, "ZH_TC.qm");
    }
    else if(type == LANG_EN)
    {
        fileConfig->set_value(LANGUAGE, "US.qm");
    }

    emit signal_change_language(language);

    // 若此时并非语言初始化，则发送重启信号
    if(isLanguageInit)
    {
        emit signal_device_reboot();
    }
    else
    {
        isLanguageInit = true;
        fileConfig->set_value(LANGUAGE_INIT, "true");
        language_init(language);
    }
}

/*******************************************************************************
*                   虎正玺@2016-09-20
* Function Name :   language_init
* Description   :   语言初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysLanguage::language_init(LanguageType language)
{
    if(language == LANG_CN)
    {
        lang = "ZH_SC.qm";
    }
    else if(language == LANG_TW)
    {
        lang = "ZH_TC.qm";
    }
    else if(language == LANG_EN)
    {
        lang = "US.qm";
    }

    //创建翻译类对象
    QTranslator *qtTranslator = new QTranslator;
    qtTranslator->load(":/resource/translation/US.qm");

    //如果从当前语言为空，则加载默认语言
//    if(lang.isEmpty())
//    {
//        qtTranslator->load(":/resource/translation/US.qm");
//    }
//    else
//    {
//        //加载配置语言
//        qtTranslator->load(QString(":/resource/translation/%1").arg(lang));
//    }

    // 配置当前所选语言
    qApp->installTranslator(qtTranslator);
}

/*******************************************************************************
* Function Name  :  slot_set_language
* Description    :  设置语言
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLanguage::slot_set_language(LanguageType type)
{
    slot_change_language(type);
    language_init(type);
}

/*******************************************************************************
* Function Name  :  get_languageType
* Description    :  获取此时语言
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
LanguageType SysLanguage::get_languageType()
{
    lang = fileConfig->get_value(LANGUAGE);

    if("ZH_SC.qm" == lang)
    {
        language = LANG_CN;
    }
    else if("ZH_TC.qm" == lang)
    {
        language = LANG_TW;
    }
    else
    {
        language = LANG_EN;
    }
    return language;
}
