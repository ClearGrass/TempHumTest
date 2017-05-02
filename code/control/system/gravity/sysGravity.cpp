/*******************************************************************************
* File Name          :  sysGravity.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  11/04/2016
* Description        :  方向管理
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "sysGravity.h"

/* Private variables ---------------------------------------------------------*/


SysGravity *SysGravity::instance = NULL;

/* Private function prototypes -----------------------------------------------*/



SysGravity *SysGravity::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysGravity();
    }
    return instance;
}

/*******************************************************************************
*                           陆超@2016-11-04
* Function Name  :  SysGravity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysGravity::SysGravity(void)
{
    qRegisterMetaType<DIRECTION>("DIRECTION");
    data_init();
    connect_init();
    moveToThread(this);

}// End of SysGravity::SysGravity(void)

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysGravity::data_init()
{
    int j;
    driverGravity = DriverGravity::getInstance();
    driverGravity->moveToThread(this);

    sysDirection = HORIZONTAL;
    isVerticalTimeout =false;
    timerSampling = new QTimer(this);
    timerSampling->setInterval(GRAVITY_SAMPLING_INTERVAL);
    timerSampling->start();

    timerVertical = new QTimer(this);
    timerVertical->setInterval(DEVICE_VERTICAL_TIMEOUT);
    timerVertical->setSingleShot(true);

    // 初始化寄存器
    driverGravity->sampling_gravity();
    lastX = driverGravity->get_x();
    lastY = driverGravity->get_y();
    lastZ = driverGravity->get_z();
    for( j = 0; j < GRAVITY_CACHE_DEPTH; j++)
    {
        listCacheX.append(driverGravity->get_x());
        listCacheY.append(driverGravity->get_y());
        listCacheZ.append(driverGravity->get_z());
    }

}

/*******************************************************************************
* Function Name  :  connect_init()
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysGravity::connect_init()
{
    connect(timerSampling, SIGNAL(timeout()), this, SLOT(slot_refresh_gravity()));
    connect(timerVertical, SIGNAL(timeout()), this, SLOT(slot_vertical_timeout()));
}

/*******************************************************************************
*                           陆超@2016-11-04
* Function Name :   refresh_gravity
* Description   :   刷新xy轴迭代数据
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysGravity::slot_refresh_gravity(void)
{
    int totalX =0;
    int totalY =0;
    int totalZ =0;
    int i;
    // 获取新数据
    driverGravity->sampling_gravity();
    listCacheX.append(driverGravity->get_x());
    listCacheY.append(driverGravity->get_y());
    listCacheZ.append(driverGravity->get_z());

    // 检测到双击
    if (driverGravity->get_click())
    {
        emit signal_shell_doubleTap();

        // 休眠防止寄存器误触发
        this->msleep(GRAVITY_SAMPLING_INTERVAL * 3);
    }

    // 删除多余列表
    while(listCacheX.length() > GRAVITY_CACHE_DEPTH)
    {
        listCacheX.removeFirst();
        listCacheY.removeFirst();
        listCacheZ.removeFirst();
    }

    // 迭代取平均
    for( i =0 ; i < GRAVITY_CACHE_DEPTH; i++)
    {
        totalX += listCacheX.at(i);
        totalY += listCacheY.at(i);
        totalZ += listCacheZ.at(i);
//        qDebug("----------------------------------------------------------z[%d] = %d z= %d\r\n", i, listCacheZ.at(i), driverGravity->get_z());
    }

    // 方向判定
    change_direction(totalX / GRAVITY_CACHE_DEPTH, totalY / GRAVITY_CACHE_DEPTH);
    change_status(totalX / GRAVITY_CACHE_DEPTH, totalY / GRAVITY_CACHE_DEPTH, totalZ / GRAVITY_CACHE_DEPTH);

    if(sysDirection == VERTICAL)
    {
        if(!timerVertical->isActive() && !isVerticalTimeout)
        {
            if(!is_up(totalX / GRAVITY_CACHE_DEPTH, totalY / GRAVITY_CACHE_DEPTH, totalZ / GRAVITY_CACHE_DEPTH))
            {
                timerVertical->start();
            }
        }
    }

}// End of void SysGravity::refresh_gravity(void)

/*******************************************************************************
*                           陆超@2016-11-04
* Function Name :   change_direction
* Description   :   方向判定
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysGravity::change_direction(int x, int y)
{
//    qDebug("x = %d  y = %d  current Direction = %d\n", x, y, sysDirection);
    if (sysDirection == VERTICAL)
    {
        if (((y < -800 ) || (y > 800)) && (x < 500))
        {
            emit signal_change_direction(HORIZONTAL);
            sysDirection = HORIZONTAL;
            qDebug("--------------纵变横------------------\n");
        }
    }
    else
    {
        if ((y > -400 ) && (y < 400) && ((x  > 500) || (x < -500)))
        {
            emit signal_change_direction(VERTICAL);
            sysDirection = VERTICAL;
            qDebug("--------------横变纵------------------\n");
        }
    }
}// End of void SysGravity::change_direction(int x, int y)

/*******************************************************************************
* Function Name  :  change_status
* Description    :  改变状态
* Input          :  int x       x轴
*                   int y       y轴
*                   int z       z轴
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysGravity::change_status(int x, int y, int z)
{
    // 若位置改动
    if((qAbs(x - lastX) > GRAVITY_X_THRESHOLD_MOVE) || (qAbs(y - lastY) > GRAVITY_X_THRESHOLD_MOVE) || (qAbs(z - lastZ) > GRAVITY_X_THRESHOLD_MOVE))
    {
        // 发送设备抬起信号
        qDebug()<<"---------------------------------------------------------DEVICE_PICK_UP";
        qDebug("--------------------------------------------------------- x = %d, lastX = %d, y = %d, lastY = %d,z = %d, lastZ = %d\r\n", x, lastX, y ,lastY, z, lastZ);
        emit signal_device_pickUp();

        // 现在属于超时提醒
        if(isVerticalTimeout)
        {
//            emit signal_device_pickUp();
            isVerticalTimeout = false;
        }
        else
        {
            timerVertical->stop();
        }
    }
    lastX = x;
    lastY = y;
    lastZ = z;

}// End of void SysGravity::change_status(int x, int y, int z)


bool SysGravity::is_up(int x, int y, int z)
{
    if((qAbs(x) < 50) && (qAbs(y) < 50) && (z < -1000))
    {
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  slot_vertical_timeout
* Description    :  竖直屏幕超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysGravity::slot_vertical_timeout()
{

    int totalX =0;
    int totalY =0;
    int totalZ =0;
    int i;

    // 迭代取平均
    for( i =0 ; i < GRAVITY_CACHE_DEPTH; i++)
    {
        totalX += listCacheX.at(i);
        totalY += listCacheY.at(i);
        totalZ += listCacheZ.at(i);

    }
    if(!is_up(totalX / GRAVITY_CACHE_DEPTH, totalY / GRAVITY_CACHE_DEPTH, totalZ / GRAVITY_CACHE_DEPTH))
    {
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@HORIZONTAL_PROMPT";
        emit signal_device_horizontalPrompt();
        isVerticalTimeout = true;
    }
}


/********************************* END OF FILE ********************************/
