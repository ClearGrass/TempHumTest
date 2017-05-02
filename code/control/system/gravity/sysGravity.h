/******************** (C) COPYRIGHT 2016 陆超 **********************************
* File Name          :  sysGravity.h
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  11/04/2016
* Description        :  sysGravity header file
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSGRAVITY_H
#define __SYSGRAVITY_H

/* Includes ------------------------------------------------------------------*/
#include <QTimer>
#include <QThread>
#include "globalVariable.h"
#include "driverGravity.h"

/* Private define ------------------------------------------------------------*/
#define     GRAVITY_SAMPLING_INTERVAL   100                             // 采样间隔 ms
#define     GRAVITY_CACHE_DEPTH         8                               // 迭代深度

#define     GRAVITY_X_THRESHOLD_MOVE    5                               // 移动判断

#define     GRAVITY_X_THRESHOLD_MAX     800                             //

#define     GRAVITY_CACHE_SUB           200
#define     GRAVITY_CACHE_DEVIATION     500

#define     DEVICE_VERTICAL_TIMEOUT     (5*1000*60)

enum DIRECTION
{
    HORIZONTAL = 0,
    VERTICAL   = 3

};

/* Private typedef -----------------------------------------------------------*/
class SysGravity : public QThread
{
    Q_OBJECT
public:
    static SysGravity *getInstance();

signals:
    void signal_device_horizontalPrompt();
    void signal_device_pickUp();
    void signal_shell_doubleTap();
    void signal_change_direction(DIRECTION);

protected slots:
    void slot_refresh_gravity();
    void slot_vertical_timeout();

private:
    SysGravity(void);
    void data_init();
    void connect_init();
    void change_direction(int x,int y);
    void change_status(int x, int y, int z);
    bool is_up(int x, int y, int z);

private:
    DriverGravity   *driverGravity;                                  // 三轴驱动

    bool isVerticalTimeout;

    int lastX;
    int lastY;
    int lastZ;

    QList<int>      listCacheX;                                     // 横坐标
    QList<int>      listCacheY;                                     // 纵坐标
    QList<int>      listCacheZ;                                     // 纵坐标
    DIRECTION   sysDirection;
    QTimer *timerSampling;
    QTimer *timerVertical;

private:
    static SysGravity  *instance;

};

/* Private variables ---------------------------------------------------------*/




#endif /* __SYSGRAVITY_H */

/*************************** END OF FILE **************************************/
