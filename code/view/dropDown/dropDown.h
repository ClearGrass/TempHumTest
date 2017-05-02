#ifndef DROPDOWN_H
#define DROPDOWN_H
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "sysControl.h"
#define QML_DROPDOWN_SOURCE_URL      "qrc:/resource/qml/Dropdown/Dropdown.qml"
class DropDown : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit DropDown(QWidget *parent = 0);

signals:
   void  signal_device_screenOff();
   void signal_set_night(bool);
   void signal_set_lightMode(bool);
   void signal_dropUp();
   void signal_set_lightValue(int);
   void signal_autoChangeValue();
   void signal_updateNightMode();
public slots:
    void slot_screen_off();
    bool slot_getNightMode();
    void slot_setNightMode(bool);                                        //设置夜间模式
    bool slot_isAuto();                                                 //获取是否自动调节亮度
    void slot_setAutoChangeLight(bool  checked);                        //设置是否自动调节亮度
    void slot_setLightValue(int value);                                 //设置亮度值
    void slot_auto_changeValue(int value);
    int slot_getLightValue();                                           //获取当前亮度值
    void slot_in_night(bool);

private:
    bool isNight;
    SysControl *sysControl;
    SysTime     *sysTime;

    int curLightValue;                                                  //当前亮度值




};

#endif // DROPDOWN_H
