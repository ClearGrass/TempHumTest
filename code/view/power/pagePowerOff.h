#ifndef PAGEPOWEROFF_H
#define PAGEPOWEROFF_H
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QTimer>

#define QML_POWER_OR_SCREEN_OFF_URL  "qrc:/resource/qml/poweroff/PoweroffPage.qml"

class PagePowerOff : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit PagePowerOff(QWidget *parent = 0);
    void show_page();

signals:
    void signal_device_screenOff();
    void signal_device_powerOff();
    void signal_horrizonScreen();
    void signal_vertialScreen();
    void signal_startTimer();
public slots:
    void slot_power_off();
    void slot_screen_off();
    void slot_cancel();

    void slot_changeHorizonScreen();
    void slot_changevertialScreen();

};

#endif // PAGEPOWEROFF_H
