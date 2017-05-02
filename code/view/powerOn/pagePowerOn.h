#ifndef PAGEPOWERON_H
#define PAGEPOWERON_H
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#define QML_POWERON_SOURCE_URL      "qrc:/resource/qml/poweron/PoweronPage.qml"
class PagePowerOn : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit PagePowerOn(QWidget *parent = 0);

signals:
    void signal_system_start();
    void signal_device_powerOff();

public slots:
    void slot_power_off();
    void slot_power_on();


};

#endif // PAGEPOWERON_H
