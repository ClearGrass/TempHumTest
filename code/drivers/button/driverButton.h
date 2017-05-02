/******************** (C) COPYRIGHT 2014 陆超 **********************************
* File Name          :  DriverButton.h
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  09/21/2016
* Description        :  DriverButton header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVERBUTTON_H
#define __DRIVERBUTTON_H

/* Includes ------------------------------------------------------------------*/
#include <QThread>
#include <QFile>
#include <QTimer>
#include "driversInit.h"


/* Private define ------------------------------------------------------------*/

#define     DEVICE_BUTTON               "/dev/button_check"
#define     BUTTON_LONG_THRESHOLD       1500                            // 长按阈值
#define     BUTTON_CLICK_THRESHOLD      1000                            // 短按阈值
#define     BUTTON_CLICK_INTERVAL       500                             // 单双击判断间隔
#define     BUTTON_FILE                 "/dev/input/event3"             // 按键文件

/* Private typedef -----------------------------------------------------------*/
class DriverButton : public QThread
{
    Q_OBJECT
public:
    static DriverButton *getInstance();

signals:
    void signal_button_longPress();
    void signal_button_singleClick();
    void signal_button_doubleClick();

protected:
    void run(void);

protected slots:
    void slot_longPress_timeout();
    void slot_doubleClick_timeout();
    void slot_singleClick_timeout();

private:
    DriverButton();
    ~DriverButton();

private:
    QTimer *timerLongPress;
    QTimer *timerSingleClick;
    QTimer *timerDoubleClick;
    DriversInit *driversInit;

    bool isDoubleClick;

private:
    static DriverButton *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverButton::instance)
                delete DriverButton::instance;
        }
    };
    static FreeInstance free;

};

/* Private variables ---------------------------------------------------------*/




#endif /* __DRIVERBUTTON_H */

/*************************** END OF FILE **************************************/
