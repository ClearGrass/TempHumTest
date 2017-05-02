#ifndef SYSBUTTON_H
#define SYSBUTTON_H


/* Includes ------------------------------------------------------------------*/
#include <QThread>
#include <QFile>
#include <QTimer>
#include "driverButton.h"
/* Private define ------------------------------------------------------------*/



/* Private typedef -----------------------------------------------------------*/

class SysButton : public QThread
{
    Q_OBJECT

public:
    static SysButton *getInstance();

signals:
    void signal_button_singleClick();
    void signal_button_doubleClick();
    void signal_button_longPress();

protected:
    SysButton();                                                        // 构造函数
    ~SysButton(void);                                                   // 析构函数

private slots:
    void slot_button_doubleClick();
    void slot_button_singleClick();
    void slot_button_longPress();

private:
    static SysButton *instance;
    DriverButton *driverButton;

};

/* Private variables ---------------------------------------------------------*/

#endif // SYSBUTTON_H

/*************************** END OF FILE **************************************/
