/*******************************************************************************
* File Name          :  sysWiFi.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  10/08/2016
* Description        :  Wi-Fi连接管理
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <QDebug>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wifi_intf.h>
#include <pthread.h>

#include "sysWiFi.h"
#include "globalVariable.h"


/* Private variables ---------------------------------------------------------*/
SysWiFi *SysWiFi::instance = NULL;

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
*                           陆超@2016-10-08
* Function Name :   getInstance
* Description   :   获取单例函数
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
SysWiFi* SysWiFi::getInstance(void)
{
    // 是否被创建过
    if(instance == NULL)
    {
        instance = new SysWiFi();
    }
    return instance;

}// End of SysWiFi* SysWiFi::getInstance()

/*******************************************************************************
*                           陆超@2016-10-08
* Function Name  :  SysWiFi
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysWiFi::SysWiFi(QObject *parent)
    :QThread(parent)
{
    qRegisterMetaType<WiFi>("WiFi");
    data_init();
    connect_init();
    moveToThread(this);

}// End of SysWiFi::SysWiFi()

/*******************************************************************************
*                           陆超@2016-11-23
* Function Name :   data_init
* Description   :   Wi-Fi管理变量初始化
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
void SysWiFi::data_init(void)
{
    driverWiFi  = DriverWiFi::getInstance();

    timerScan   = new QTimer();
    timerUpdate = new QTimer();

    // 15S进行一次扫描，15S进行一次刷新
    timerScan->setInterval(WIFI_SCAN_INTERVAL);
    timerUpdate->setInterval(WIFI_UPDATE_INTERVAL);

    // 初始化此时的Wi-Fi列表和Cookie列表
    listWiFi        = new QList<WiFi>;
    listCookie      = new QList<Cookie>;

    // 此时的Wi-Fi状态为未连接，网络状态为离线
    wifiStatus      = WIFI_DISCONNECT;
    netStatus       = NET_OFFLINE;

    // 此时还没有扫描
    isScaned        = false;

    // 当前WiFi初始化
    currentWiFi.init();

}// End of void SysWiFi::data_init(void)

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::connect_init()
{
    connect(timerScan, SIGNAL(timeout()), this, SLOT(slot_scan_wifi()));
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slot_update_wifi()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  运行该线程
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::run()
{

#ifdef Bran_R8
    // 打开WiFi
    if(driverWiFi->WiFi_on() != 0)
    {
        while(1)
        {
            qDebug("打开Wi-Fi失败\n");
            sleep(1);
        }
    }
    else
    {
        qDebug("OPEN Wi-Fi  SUCCESS\n");
    }

    // 首次刷新 提高实时性
    driverWiFi->WiFi_scan();

    // 扫描缓存表
    scan_cookie();

    // 扫描当前Wi-Fi列表
    slot_scan_wifi();

    isScaned = true;

    // 首次同步状态 防止Wi-Fi打开后自动连接
    wifiStatus = get_wifiStatus();

    // 若此时的Wi-Fi连接上的
    if(WIFI_CONNECTED == wifiStatus)
    {
        // 获取此时的SSID
        currentWiFi.ssid = get_wifiSSID();
        netStatus = NET_ONLINE;

        // 此时WiFi状态为已连接且可用
        currentWiFi.status = CONNECTED;

        // 获取此时的信号强度
        currentWiFi.signalNum = get_wifiSignalNum(currentWiFi.ssid);
        currentWiFi.ip        = get_wifiIP();

        // 发送WiFi刷新信号
        emit signal_update_wifi(currentWiFi);
        qDebug()<<"######################################################WIFI";
    }
    slot_refresh_listWiFi();
    timerScan->start();
    timerUpdate->start();

#endif

    this->exec();
}

/*******************************************************************************
* Function Name  :  get_currentWiFi
* Description    :  获取当前WiFi
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WiFi SysWiFi::get_currentWiFi()
{
    return currentWiFi;
}

/*******************************************************************************
* Function Name :   slot_scan_wifi()
* Description   :   正常刷新Wi-Fi信息
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::slot_scan_wifi()
{
    qDebug() << "************************** WiFi Scan **************************";

    // 扫描当前wifi，并把扫描结果赋值给scan_resilts
    int i;
    int len = 0;
    int index;
    char scan_results[4096];
    QStringList wifiInfo;
    WiFi wifi;
    QString strKeymgmt;

    driverWiFi->WiFi_scan_results(scan_results, &len);

    QString results;
    results += scan_results;

    // listWiFi里有数值的情况下，清空容器
    if(listWiFi->length() != 0)
    {
        listWiFi->clear();
    }

    // 扫描结果按行分隔并存入listResults
    QStringList listResults = results.split("\n");
    for(i = 1; i<listResults.length();++i)
    {
        // 以制表符分隔，并存入字符串wifiInfo
        wifiInfo = listResults.at(i).split("\t");
        if(wifiInfo.length() != 5)
        {
            // 当长度不为5，则不为WiFi数据行
            continue;
        }

        // Wi-Fi名称
        wifi.ssid = wifiInfo[4].trimmed();

        wifi.ssid.replace("\\\\","\\");

        // 转换为中文SSID
        if((wifi.ssid).contains("\\x"))
        {
            wifi.ssid = trans_utf8(wifi.ssid);
        }

        // Wi-Fi信号强度
        wifi.strength  = wifiInfo[2].trimmed().toInt();

        // 判断之前是否已存在该WiFi
        index = get_wifiIndex(wifi.ssid);
        if(index != -1)
        {
            // 若上一个相同SSID的Wi-Fi信号强度大于此时Wi-Fi的信号强度
            if(listWiFi->at(index).strength >= wifi.strength)
            {
                // 跳过
                continue;
            }
            else
            {
                // 删除已有的SSID
                listWiFi->removeAt(index);
            }
        }

        // Wi-Fi 信号格数
        wifi.signalNum = trans_signalNum(wifi.strength);

        // 加密方式栏
        strKeymgmt = wifiInfo[3].trimmed();

        wifi.keymgmt = trans_keymgmt(strKeymgmt);

        // 若WiFi加密方式字符串中包含WPA/WEP关键字，则认定此Wi-Fi已经加密
        wifi.lock = (wifi.keymgmt == WIFI_KEY_NONE)? false : true;

        // 默认情况下此Wi-Fi为断开状态；
        wifi.status = DISCONNECT;

        // 测试所用,判断是否为已存WiFi
        wifi.key = is_cookie(wifi.ssid);
        listWiFi->append(wifi);
    }
    slot_refresh_listWiFi();

    qDebug() << "****************************End Scan WiFi**********************";
}

/*******************************************************************************
* Function Name  :  slot_update_wifi
* Description    :  刷新WiFi
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_update_wifi()
{
    // 获取此时的WiFi状态
    wifiStatus = get_wifiStatus();

    // 若为未连接
    if (WIFI_CONNECTED == wifiStatus)
    {
        if(currentWiFi.status == CONNECTED)
        {
            currentWiFi.signalNum = get_wifiSignalNum(currentWiFi.ssid);
            qDebug()<<"WIFI@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<currentWiFi.ssid;
            emit signal_update_wifi(currentWiFi);
        }
    }
    else if(WIFI_DISCONNECT == wifiStatus)
    {
        if(currentWiFi.status == CONNECTED)
        {
            currentWiFi.status = DISCONNECT;
            qDebug()<<"WIFI@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<currentWiFi.ssid;
            emit signal_update_wifi(currentWiFi);
            slot_refresh_listWiFi();
        }
        // 自动匹配
        connect_autoMatch();
    }
}

/*******************************************************************************
* Function Name  :  slot_net_abnormal
* Description    :  网络异常
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_net_abnormal()
{
    if(currentWiFi.status == CONNECTED)
    {
        // 获取此时的WiFi状态
        wifiStatus = get_wifiStatus();

        // 已连接网络
        if(WIFI_CONNECTED == wifiStatus)
        {
           netStatus = get_netStatus();
           if(NET_ONLINE == netStatus)
           {
                return;
           }
           else
           {
               currentWiFi.status = UNUSEABLE;
               emit signal_update_wifi(currentWiFi);
               slot_refresh_listWiFi();
           }

        }
        else
        {
            currentWiFi.status = DISCONNECT;
            emit signal_update_wifi(currentWiFi);
            slot_refresh_listWiFi();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_refresh_listWiFi
* Description    :  刷新WiFi列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_refresh_listWiFi()
{
    qDebug()<<"*************************Refresh WiFi**************************";
    int i;
    WiFi wifi;
    QList<WiFi> *listWiFiCache = new QList<WiFi>;
    if(!isScaned)
    {
        return;
    }

    for(i = 0; i < listWiFi->length(); ++i)
    {
        wifi.ssid = listWiFi->at(i).ssid;
        wifi.strength  = listWiFi->at(i).strength;
        wifi.signalNum = listWiFi->at(i).signalNum;
        wifi.keymgmt = listWiFi->at(i).keymgmt;
        wifi.lock = listWiFi->at(i).lock;
        wifi.key = is_cookie(wifi.ssid);
        wifi.status = DISCONNECT;

        // 若遍历到的Wi-Fi名等于目前Wi-Fi名
        if(listWiFi->at(i).ssid == currentWiFi.ssid)
        {
            wifi.status = currentWiFi.status;
        }
        listWiFiCache->append(wifi);
    }
    // 排序
    qSort(listWiFiCache->begin(),listWiFiCache->end(),compare_wifi);
    emit signal_update_listWiFi(listWiFiCache);
    qDebug()<<"***************************End Refresh WiFi********************";
}

/*******************************************************************************
* Function Name :   scan_cookie
* Description   :   扫描已有WiFi列表
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::scan_cookie()
{
    qDebug()<<"*************************SCAN LIST COOKIE***********************";
    char scan_results[4096];

    // 从后台获取当前已存cookies信息,合并到listCookie容器
    driverWiFi->WiFi_listNetworks(scan_results,4096);
    QString results;
    results += scan_results;
    qDebug()<<results;

    // 将扫描结果分行
    QStringList resultsRow = results.split("\n");
    listCookie->clear();

    // 若扫描结果不为空
    if(results.contains("ssid"))
    {
        for(int i =0; i< resultsRow.length();++i)
        {
            // 若为首行表头，跳到下一行
            if(resultsRow.at(i).contains("/"))
            {
                continue;
            }
            else
            {
                // 按制表符分隔存入QStringList
                QStringList cookieList = resultsRow.at(i).split("\t");
                Cookie newWiFi;
                newWiFi.ssid = cookieList.at(1).trimmed();
                newWiFi.flags = cookieList.at(3).trimmed();
                listCookie->append(newWiFi);
            }
        }
    }
    qDebug()<<"************************END SCAN COOKIE************************";
}

/*******************************************************************************
* Function Name :   connect_autoMatch
* Description   :   自动匹配连接
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::connect_autoMatch()
{
    qDebug()<<"***********************Connect Auto Match**********************";
    int i, j;
    QString strSSID;
    QByteArray ba1;
    char* SSID = NULL;
    int keymgmt, resultConnect;
    char netID[255];

    // 通过Cookie列表与当前WiFi列表进行遍历匹配，自动登录
    for(i = 0; i < listWiFi->length(); i++)
    {
        for(j = 0; j < listCookie->length(); j++)
        {
            // 若listCookie中的wifi名与listWiFi中的wifi名相同的的话，尝试连接
            if(listCookie->at(j).ssid == listWiFi->at(i).ssid)
            {
                // 获取缓存的SSID
                strSSID  = listCookie->at(j).ssid;
                ba1      = strSSID.toLatin1();
                SSID     = ba1.data();

                // 获取加密方式
                keymgmt  = get_wifiKeymgmt(listWiFi->at(i).ssid);

                // 当没有查询到对应的netID时
                if (driverWiFi->WiFi_gitNetID(SSID, keymgmt, netID) != 0)
                {
                    continue;
                }
                resultConnect = driverWiFi->WiFi_connectAP_withNetID(netID);
                if (WIFIMG_NETWORK_CONNECTED == resultConnect)
                {
                    qDebug() << "CONNECT SUCCESS!!!";
                    wifiStatus = WIFI_CONNECTED;
                    currentWiFi.ssid = listWiFi->at(i).ssid;
                    currentWiFi.ip   = get_wifiIP();
                    currentWiFi.status = CONNECTED;
                    currentWiFi.signalNum = get_wifiSignalNum(currentWiFi.ssid);

                    // 发送信号
                    emit signal_update_wifi(currentWiFi);
                    slot_refresh_listWiFi();
                }
                // 密码错误
                else if(WIFIMG_PASSWORD_FAILED == resultConnect)
                {
                    // 连接失败
                    wifiStatus = WIFI_DISCONNECT;

                    // 删除该项Wi-Fi
                    remove_cookie(strSSID);
                    continue;
                }
                else
                {
                    qDebug() << "-------------------------------------------断线自动重连失败";
                    wifiStatus = WIFI_DISCONNECT;
                    continue;
                }
            }
        }
    }
    qDebug()<<"*********************End Auto Match****************************";
}


/*******************************************************************************
* Function Name :   slot_connect_withKey
* Description   :   手动连接密码槽函数
* Input         :   QString ssid    目标SSID
*                   QString key
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::slot_connect_withKey(QString ssid, QString key)
{
    timerScan->stop();
    qDebug()<<"SLOT_CONNECT_WITH_KEY!!!!!!!!!!!!!"<<ssid;

    // 将WiFi状态置为正在连接
    currentWiFi.ssid   = ssid;
    currentWiFi.status = CONNECTTING;

    emit signal_update_wifi(currentWiFi);

    // 刷新此时的列表
    slot_refresh_listWiFi();

    // 若当前WiFi已连接
    if(WIFI_CONNECTED == wifiStatus)
    {
        // 断开连接且进入disconnect下连接新Wi-Fi
        if (driverWiFi->WiFi_disconnectAP() == 0)
        {
            // Wi-Fi 置为正在连接
            wifiStatus  = WIFI_DISCONNECT;

            // 循环状态置为连接新 Wi-Fi
            connect_withKey(ssid, key);
        }
        else
        {
            endless_loop("切换Wi-Fi时关闭失败");
        }
    }
    else
    {
        // 连接新WiFi
        connect_withKey(ssid, key);
    }

    timerScan->start();
}

/*******************************************************************************
* Function Name  :  slot_connect_publicWiFi
* Description    :  连接公有WiFi
* Input          :  QString ssid     连接的WiFi名
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_connect_publicWiFi(QString ssid)
{
    timerScan->stop();
    qDebug()<<"*******************Connect Public WiFi*************************";

    // 将WiFi状态置为正在连接
    currentWiFi.ssid   = ssid;
    currentWiFi.status = CONNECTTING;

    // 更新此时Wi-Fi状态
    emit signal_update_wifi(currentWiFi);
    slot_refresh_listWiFi();

    // 若当前WiFi已连接
    if(WIFI_CONNECTED == wifiStatus)
    {
        // 断开连接且进入disconnect下连接新Wi-Fi
        if (driverWiFi->WiFi_disconnectAP() == 0)
        {
            // Wi-Fi 置为未连接
            wifiStatus  = WIFI_DISCONNECT;

            // 循环状态置为连接新 Wi-Fi
            connect_withKey(ssid, "");
        }
        else
        {
            endless_loop("切换Wi-Fi时关闭失败");
        }
    }
    else
    {
        connect_withKey(ssid, "");
    }
    timerScan->start();
}


/*******************************************************************************
* Function Name :   connect_withKey
* Description   :   手动密码连接Wi-Fi
* Input         :   QString strSSID 将要连接的SSID
*                   QString strKey  将要连接的密码
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::connect_withKey(QString strSSID, QString strKey)
{
    qDebug()<<"*****************Connect With Key******************************"<<strSSID<<strKey;

    int resultConnect;
    QByteArray ba1 = strSSID.toLatin1();
    char* SSID = ba1.data();
    QByteArray ba2 = strKey.toLatin1();
    char* key = ba2.data();

    // 获取此时加密方式
    int keymgmt = get_wifiKeymgmt(strSSID);

    // 开始尝试连接
    resultConnect = driverWiFi->WiFi_addNetwork(SSID, key, keymgmt);
    qDebug()<<"RESULT>>>>>>>>>>>>>>>>>>>>"<<resultConnect;
    if(WIFIMG_NETWORK_CONNECTED == resultConnect)
    {
        // 状态设置为不可用，等待网络检测
        wifiStatus = WIFI_CONNECTED;

        currentWiFi.ssid = strSSID;
        currentWiFi.ip   = get_wifiIP();
        currentWiFi.status = CONNECTED;
        currentWiFi.signalNum = get_wifiSignalNum(currentWiFi.ssid);

        scan_cookie();

        emit signal_connect_result(WIFI_CONNECT_SUCCESS);
        emit signal_update_wifi(currentWiFi);

        // 刷新此时的界面
        slot_refresh_listWiFi();
    }
    else if(WIFIMG_PASSWORD_FAILED == resultConnect)
    {
        // 连接失败
        wifiStatus = WIFI_DISCONNECT;

        currentWiFi.status = DISCONNECT;
        currentWiFi.ip   = get_wifiIP();

        // 发送密码错误信号
        emit signal_connect_result(WIFI_PASSWORD_FAILED);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();

    }
    else
    {
        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        currentWiFi.ip   = get_wifiIP();

        // 发送连接超时信号
        emit signal_connect_result(WIFI_CONNECT_TIMEOUT);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();
    }

    qDebug()<<"**********************End Connect Key**************************";
}


/*******************************************************************************
* Function Name  :  slot_connect_withNetID
* Description    :  连接已存WiFi
* Input          :  QString ssid    已存WiFi名
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_connect_withNetID(QString strSSID)
{
    qDebug()<<"*******************Connect With NetID***************************";
    int resultGetNetID;
    int resultConnect;
    char netID[255];
    QByteArray ba;
    char *SSID;
    int keymgmt;

    // 赋值SSID
    if(WIFI_CONNECTED == wifiStatus)
    {
        if(currentWiFi.ssid == strSSID)
        {
            emit signal_connect_result(WIFI_CONNECT_SUCCESS);
            return;
        }
        else
        {
            // 将WiFi状态置为正在连接
            currentWiFi.ssid   = strSSID;
            currentWiFi.status = CONNECTTING;
            emit signal_update_wifi(currentWiFi);
            slot_refresh_listWiFi();

            // 如果存在SSID则断开连接且进入手动连接已保存Wi-Fi
            if (driverWiFi->WiFi_disconnectAP() == 0)
            {
                // Wi-FI 置为未连接
                wifiStatus  = WIFI_DISCONNECT;
            }
            else
            {
                endless_loop("切换Wi-Fi时关闭失败");
            }
        }
    }
    ba = strSSID.toLatin1();
    SSID = ba.data();

    // 获取此时加密方式
    keymgmt = get_wifiKeymgmt(strSSID);
    resultGetNetID = driverWiFi->WiFi_gitNetID(SSID, keymgmt, netID);

    // 若获取NetID失败
    if(-1 == resultGetNetID)
    {
        qDebug()<<"ERROR>>>>>>>>GET NET_ID FAILED!!!";

        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        currentWiFi.ip   = get_wifiIP();

        // 发送连接超时信号
        emit signal_connect_result(WIFI_CONNECT_TIMEOUT);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();
        return;
    }

    // 开始尝试连接
    resultConnect = driverWiFi->WiFi_connectAP_withNetID(netID);
    qDebug()<<"RESULT>>>>>>>>>>>>>>>>>>>>"<<resultConnect;

    // 连接成功
    if(WIFIMG_NETWORK_CONNECTED == resultConnect)
    {
        // 设置WiFi状态为已连接
        wifiStatus = WIFI_CONNECTED;
        currentWiFi.ssid = strSSID;
        currentWiFi.ip   = get_wifiIP();
        currentWiFi.status = CONNECTED;
        currentWiFi.signalNum = get_wifiSignalNum(currentWiFi.ssid);

        // 发送连接Success信号
        emit signal_connect_result(WIFI_CONNECT_SUCCESS);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();
    }
    else if(WIFIMG_PASSWORD_FAILED == resultConnect)
    {
        // 连接失败
        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        currentWiFi.ip   = get_wifiIP();

        // 删除此时的已存WiFi
        remove_cookie(strSSID);

        // 发送密码错误信号
        emit signal_connect_result(WIFI_PASSWORD_FAILED);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();
    }
    else
    {
        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        currentWiFi.ip   = get_wifiIP();

        // 发送连接超时信号
        emit signal_connect_result(WIFI_CONNECT_TIMEOUT);
        emit signal_update_wifi(currentWiFi);
        slot_refresh_listWiFi();
    }
}

/*******************************************************************************
* Function Name  :  slot_remove_cookie
* Description    :  忘记密码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_remove_cookie(QString ssid)
{
    qDebug()<<"Remove Cookie!!!!!!!!!!!!"<<ssid;
    // 断开连接且进入disconnect下连接新Wi-Fi
    if (driverWiFi->WiFi_disconnectAP() == 0)
    {
        // Wi-Fi 置为未连接
        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        emit signal_update_wifi(currentWiFi);

        // 删除当前Wi-Fi密码
        int result = remove_cookie(ssid);

        if(WIFI_MANAGER_SUCCESS == result)
        {
            emit signal_remove_result(WIFI_REMOVE_SUCCESS);
        }
        else
        {
            emit signal_remove_result(WIFI_REMOVE_FAILED);
        }

        slot_refresh_listWiFi();
    }
    else
    {
        endless_loop("忘记密码时，Wi-Fi关闭失败");
    }
}

/*******************************************************************************
* Function Name :   remove_cookie
* Description   :   在线态管理
* Input         :   QString strSSID 要删除的Wi-Fi名称
* Output        :   None
* Return        :   None
*******************************************************************************/
int SysWiFi::remove_cookie(QString strSSID)
{
    qDebug()<<"***************************Remove Cookie************************"<<strSSID;
    QByteArray ba1 = strSSID.toLatin1();
    char* SSID = ba1.data();
    int keymgmt = get_wifiKeymgmt(strSSID);
    int resultRemove = driverWiFi->WiFi_remove_network(SSID, keymgmt);
    scan_cookie();
    return resultRemove;
    qDebug()<<"****************************End Remove Cookie******************";
}

/*******************************************************************************
* Function Name  :  slot_remove_allCookies
* Description    :  清空所有缓存
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysWiFi::slot_remove_allCookies()
{
    if (driverWiFi->WiFi_disconnectAP() == 0)
    {
        // Wi-Fi 置为未连接
        wifiStatus = WIFI_DISCONNECT;
        currentWiFi.status = DISCONNECT;
        emit signal_update_wifi(currentWiFi);

        // 删除所有已有Wi-Fi
        remove_allCookies();

        slot_refresh_listWiFi();
    }
    else
    {
        endless_loop("删除所有已有Wi-Fi时，Wi-Fi关闭失败");
    }
}

/*******************************************************************************
* Function Name :   remove_allCookies
* Description   :   移除所有WiFi密码
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
int SysWiFi::remove_allCookies()
{
    qDebug()<<"*********************Remove All Cookies************************";
    driverWiFi->WiFi_remove_all_networks();
    scan_cookie();
    qDebug()<<"**********************End Remove Cookies***********************";
}

/*******************************************************************************
* Function Name  :  get_wifiStatus
* Description    :  获取WiFi状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int SysWiFi::get_wifiStatus()
{
    if(driverWiFi->get_wifi_state() == WIFIMG_WIFI_CONNECTED)
    {
        // 将Wi-Fi状态置为已连接
        return WIFI_CONNECTED;
    }
    else
    {
        // 若不是已连接，目前都定义为未连接；
        return WIFI_DISCONNECT;
    }
}

/*******************************************************************************
* Function Name  :  get_netStatus
* Description    :  获取此时的网络状态
* Input          :  None
* Output         :  None
* Return         :  网络是否连接
*******************************************************************************/
int SysWiFi::get_netStatus()
{
    qDebug()<<"**********************START PING *******************************";
    QProcess *process = new QProcess;
    process->start("ping -c 3 www.baidu.com");
    process->waitForStarted();
    process->waitForFinished();
    QString str = QString::fromLocal8Bit(process->readAllStandardOutput());
    if(process)
    {
        process->close();
        delete process;
        process = 0;
    }
    qDebug()<<"***********************END PING*********************************";
    qDebug()<<str;
    if(str.contains("time="))
    {
        return NET_ONLINE;
    }
    return NET_OFFLINE;
}

/*******************************************************************************
* Function Name  :  get_keyType
* Description    :  获取WiFi类型
* Input          :  QString ssid WiFi名
* Output         :  None
* Return         :  类型
*******************************************************************************/
int SysWiFi::get_keyType(QString ssid)
{
    qDebug()<<"COOKIE LIST"<<ssid<<listCookie->length();

    for(int j = 0; j < listWiFi->length(); ++j)
    {
        if(listWiFi->at(j).ssid == ssid)
        {
            if(!listWiFi->at(j).lock)
            {
                return WIFI_NO_ENCRYPT;
            }
            break;
        }
    }
    if(listCookie->isEmpty())
    {
        return WIFI_NO_KEY;
    }
    for(int i =0;i<listCookie->length();++i)
    {
        if(listCookie->at(i).ssid == ssid)
        {
            return WIFI_SAVED_KEY;
        }
    }
    return WIFI_NO_KEY;
}

/*******************************************************************************
* Function Name  :  is_cookie
* Description    :  是否已存
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysWiFi::is_cookie(const QString ssid)
{
    for(int i = 0; i < listCookie->length(); ++i)
    {
        if(listCookie->at(i).ssid == ssid)
        {
            return true;
        }
    }
    return false;
}

/*******************************************************************************
* Function Name  :  get_wifiIndex
* Description    :  获取此时的索引
* Input          :  QString ssid WiFi名称
* Output         :  None
* Return         :  None
*******************************************************************************/
int SysWiFi::get_wifiIndex(QString ssid)
{
    for(int i = 0; i <listWiFi->length();++i)
    {
        if(listWiFi->at(i).ssid == ssid)
        {
            return i;
        }
    }
    return -1;
}

/*******************************************************************************
* Function Name  :  get_wifiSignalNum
* Description    :  获取WiFi信号强度
* Input          :  QString ssid 检索的SSID
* Output         :  None
* Return         :  信号格数
*******************************************************************************/
int SysWiFi::get_wifiSignalNum(QString ssid)
{
    for(int i = 0; i <listWiFi->length();++i)
    {
        if(listWiFi->at(i).ssid == ssid)
        {
            return listWiFi->at(i).signalNum;
        }
    }
    return 0;
}

/*******************************************************************************
* Function Name  :  get_wifiSSID
* Description    :  获取当前SSID名称
* Input          :  None
* Output         :  None
* Return         :  SSID
*******************************************************************************/
QString SysWiFi::get_wifiSSID()
{
    if(listCookie->isEmpty())
    {
        scan_cookie();
    }

    // 若当前WiFi可联网，则获取WiFi信号强度
    for(int i = 0; i < listCookie->length(); ++i)
    {
        // 若SSID匹配，获取信号强度
        if(listCookie->at(i).flags.contains("CURRENT"))
        {
            // 返回此时的信号格数
            return listCookie->at(i).ssid;
        }
    }
    return "";
}

/*******************************************************************************
* Function Name  :  get_wifiIP
* Description    :  获取当前IP名称
* Input          :  None
* Output         :  None
* Return         :  SSID
*******************************************************************************/
QString SysWiFi::get_wifiIP()
{
    char cIP[50];
    char cMAC[50];
    if(0 == driverWiFi->WiFi_Get_MAC_IP(cMAC, cIP))
    {
        return QString(cIP);
    }
    else
    {
        return "127.0.0.1";
    }
}

/*******************************************************************************
*                           陆超@2016-11-23
* Function Name :   endless_loop
* Description   :   调试死循环
* Input         :   const char * debugData 打印信息
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysWiFi::endless_loop(const char * debugData)
{
    while(1)
    {
        sleep(1);
        qDebug() << debugData;
    }

}// End of void SysWiFi:: endless_loop(const char * debugData)




/*******************************************************************************
* Function Name :   compare_wifi(const WiFi wifi1, const WiFi wifi2)
* Description   :   自定义WiFi列表排序方式
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool SysWiFi::compare_wifi(const WiFi wifi1, const WiFi wifi2)
{
    // 若wifi1状态为已连接，wifi2为未连接，则wifi1排在wifi2前面
    if((wifi1.status != DISCONNECT)&&(wifi2.status == DISCONNECT))
    {
        return true;
    }

    // 若wifi1为未连接，而wifi2已连接，则wifi2排在wifi1前面
    else if((wifi1.status == DISCONNECT)&&(wifi2.status != DISCONNECT))
    {
        return false;
    }
    else
    {
        // 当wifi1和wifi2都已连接或者都没连接时，将信号强度更强的wifi排在前面
        if (wifi1.strength < wifi2.strength)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

/*******************************************************************************
* Function Name  :  trans_keymgmt
* Description    :  加密方式分类
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int SysWiFi::trans_keymgmt(QString strKeymgmt)
{
    int key = 0;
    if(strKeymgmt.contains("WPA"))
    {
        key = WIFI_KEY_WPA_PSK;
    }
    else if(strKeymgmt.contains("WEP"))
    {
        key = WIFI_KEY_WEP;
    }
    else
    {
        key = WIFI_KEY_NONE;
    }
    return key;
}

/*******************************************************************************
* Function Name  :  trans_signalNum
* Description    :  信号强度转换为信号格数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int SysWiFi::trans_signalNum(int strength)
{

    int signalNum = 0;
    if(strength > WIFI_SIGNAL_STRENGTH_4)
    {
        signalNum = 4;
    }
    else if(strength>WIFI_SIGNAL_STRENGTH_3)
    {
        signalNum = 3;
    }
    else if(strength>WIFI_SIGNAL_STRENGTH_2)
    {
        signalNum = 2;
    }
    else
    {
        signalNum = 1;
    }
    return signalNum;
}

/*******************************************************************************
* Function Name  :  trans_utf8
* Description    :  中文SSID转换
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SysWiFi::trans_utf8(QString gbk)
{
    QString strUTF8;
    string SubStr;
    QStringList listSSID = gbk.split("\\x");

    for(int i = 1; i < listSSID.length(); ++i)
    {
        SubStr = (listSSID.at(i).mid(0,2)).toStdString();
        wchar_t ws[2] = { 0 };
        sscanf(SubStr.c_str(), "%x", &ws);
        strUTF8 += QString::fromWCharArray(ws);
        strUTF8 += listSSID.at(i).mid(2);
        qDebug()<<QString::fromStdString(SubStr);
    }
    return QString::fromUtf8(strUTF8.toLatin1().data());
}

/*******************************************************************************
* Function Name :   get_wifiKeymgmt
* Description   :   获取指定WiFi的加密方式
* Input         :   QString ssid 需要加密方式的SSID
* Output        :   None
* Return        :   None
*******************************************************************************/
int SysWiFi::get_wifiKeymgmt(QString ssid)
{
    int i = 0;
    for(i = 0; i < listWiFi->length(); ++i)
    {
        if(listWiFi->at(i).ssid == ssid)
        {
            return listWiFi->at(i).keymgmt;
        }
    }
    return -1;
}

/********************************* END OF FILE ********************************/




