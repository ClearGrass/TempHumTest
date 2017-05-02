/*******************************************************************************
* File Name          :  driverWiFi.cpp
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator 4.10
* Version            :  V1.0
* Date               :  09/30/2016
* Description        :  Wi-Fi驱动
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <QDebug>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wifi_intf.h>
#include <pthread.h>

#include "driverWiFi.h"
#include "globalVariable.h"

DriverWiFi *DriverWiFi::instance = NULL;
/* Private variables ---------------------------------------------------------*/

const char *WiFi_statusTable[]{
    "WIFIMG_WIFI_ON_SUCCESS",
    "WIFIMG_WIFI_ON_FAILED",
    "WIFIMG_WIFI_OFF_SUCCESS",
    "WIFIMG_WIFI_OFF_FAILED",
    "WIFIMG_NO_NETWORK_CONNECTING",
    "WIFIMG_CMD_OR_PARAMS_ERROR",
    "WIFIMG_KEY_MGMT_NOT_SUPPORT",
    "WIFIMG_OPT_NO_USE_EVENT",
    "WIFIMG_NETWORK_NOT_EXIST",
    "WIFIMG_DEV_BUSING_EVENT",
    "WIFIMG_NETWORK_DISCONNECTED",
    "WIFIMG_NETWORK_CONNECTED",
    "WIFIMG_PASSWORD_FAILED",
    "WIFIMG_CONNECT_TIMEOUT"
};

const char *WiFi_stateTable[]{
    "WIFIMG_WIFI_ENABLE",
    "WIFIMG_WIFI_DISCONNECTED",
    "WIFIMG_WIFI_BUSING",
    "WIFIMG_WIFI_CONNECTED",
    "WIFIMG_WIFI_DISABLED"
};

tWIFI_EVENT eventWiFi = WIFIMG_NETWORK_DISCONNECTED;
int callBack_label = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           陆超@2016-10-08
* Function Name  :  wifi_event_handle
* Description    :  Wi-Fi状态机
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void wifi_event_handle(tWIFI_EVENT wifi_event, void *buf, int event_label)
{
    qDebug("event_label 0x%x\n", event_label);

    switch(wifi_event)
    {
        case WIFIMG_WIFI_ON_SUCCESS:
        {
            qDebug("WiFi on success!\n");
            eventWiFi = WIFIMG_WIFI_ON_SUCCESS;
            break;
        }

        case WIFIMG_WIFI_ON_FAILED:
        {
            qDebug("WiFi on failed!\n");
            eventWiFi = WIFIMG_WIFI_ON_FAILED;
            break;
        }

        case WIFIMG_WIFI_OFF_FAILED:
        {
            qDebug("wifi off failed!\n");
            eventWiFi = WIFIMG_WIFI_OFF_FAILED;
            break;
        }

        case WIFIMG_WIFI_OFF_SUCCESS:
        {
            qDebug("wifi off success!\n");
            eventWiFi = WIFIMG_WIFI_OFF_SUCCESS;
            break;
        }

        case WIFIMG_NETWORK_CONNECTED:
        {
            qDebug("WiFi connected ap!\n");
            eventWiFi = WIFIMG_NETWORK_CONNECTED;
            break;
        }

        case WIFIMG_NETWORK_DISCONNECTED:
        {
            qDebug("WiFi disconnected!\n");
            eventWiFi = WIFIMG_NETWORK_DISCONNECTED;
            break;
        }

        case WIFIMG_PASSWORD_FAILED:
        {
            qDebug("Password authentication failed!\n");
            eventWiFi = WIFIMG_PASSWORD_FAILED;
            break;
        }

        case WIFIMG_CONNECT_TIMEOUT:
        {
            qDebug("Connected timeout!\n");
            eventWiFi = WIFIMG_CONNECT_TIMEOUT;
            break;
        }

        case WIFIMG_NO_NETWORK_CONNECTING:
        {
              qDebug("It has no wifi auto connect when wifi on!\n");
              eventWiFi = WIFIMG_NO_NETWORK_CONNECTING;
              break;
        }

        case WIFIMG_CMD_OR_PARAMS_ERROR:
        {
            qDebug("cmd or params error!\n");
            eventWiFi = WIFIMG_CMD_OR_PARAMS_ERROR;
            break;
        }

        case WIFIMG_KEY_MGMT_NOT_SUPPORT:
        {
            qDebug("key mgmt is not supported!\n");
            eventWiFi = WIFIMG_KEY_MGMT_NOT_SUPPORT;
            break;
        }

        case WIFIMG_OPT_NO_USE_EVENT:
        {
            qDebug("operation no use!\n");
            eventWiFi = WIFIMG_OPT_NO_USE_EVENT;
            break;
        }

        case WIFIMG_NETWORK_NOT_EXIST:
        {
            qDebug("network not exist!\n");
            eventWiFi = WIFIMG_NETWORK_NOT_EXIST;
            break;
        }

        case WIFIMG_DEV_BUSING_EVENT:
        {
            qDebug("wifi device busing!\n");
            eventWiFi = WIFIMG_DEV_BUSING_EVENT;
            break;
        }

        default:
        {
            qDebug("Other event, no care!\n");
        }
    }

    // 同步事件
    callBack_label = event_label;

}// End of void wifi_event_handle(tWIFI_EVENT wifi_event, void *buf, int event_label)


/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverWiFi* DriverWiFi::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverWiFi();
    }
    return instance;
}

/*******************************************************************************
*                           陆超@2016-09-30
* Function Name  :  DriverWiFi
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverWiFi::DriverWiFi()
{

#ifdef Bran_R8

    qDebug("\r\n进入Wi-Fi驱动线程!\r\n");



#endif

}// End of DriverWiFi::DriverWiFi()

/*******************************************************************************
*                           陆超@2016-09-30
* Function Name :   ~DriverWiFi
* Description   :   析构函数
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
DriverWiFi::~DriverWiFi(void)
{
#ifdef Bran_R8

#endif

}// End of DriverWiFi::~DriverWiFi(void)

/*******************************************************************************
*                           陆超@2016-09-30
* Function Name :   WiFi_scan
* Description   :   Wi-Fi扫描
* Input         :   void
* Output        :   None
* Return        :   -1 扫描失败 0 扫描成功
*******************************************************************************/
int DriverWiFi::WiFi_scan(void)
{

#ifdef Bran_R8

    int event_label;

    event_label = rand();

    if (p_wifi_interface->start_scan(event_label) == 0)
    {
        qDebug() << "完成Scan" ;

        return 0;
    }
    else
    {
        qDebug() << "Scan失败" ;

        return -1;
    }

#endif


}// End of int DriverWiFi::WiFi_scan(void)

/*******************************************************************************
*                           陆超@2016-11-29
* Function Name :   WiFi_scan_results
* Description   :   获取扫描结果
* Input         :   char *scan_results  扫描结果
*                   int *len            长度
* Output        :   None
* Return        :   0 读取成功
*******************************************************************************/
int DriverWiFi::WiFi_scan_results(char *scan_results, int *len)
{

#ifdef Bran_R8

    *len = 4096;
    p_wifi_interface->get_scan_results(scan_results, len);

    qDebug() << "wifi scan result" << scan_results;

    return 0;

#endif


}// End of int DriverWiFi::WiFi_scan_results(char *scan_results, int *len)

/*******************************************************************************
*                           陆超@2016-09-30
* Function Name :   WiFi_on
* Description   :   打开WiFi
* Input         :   None
* Output        :   None
* Return        :   -1 打开失败 0 打开成功
*******************************************************************************/
int DriverWiFi::WiFi_on(void)
{
#ifdef Bran_R8

    int event_label;

    event_label = rand();

    p_wifi_interface = NULL;
    p_wifi_interface = aw_wifi_on(wifi_event_handle, event_label);
    if(p_wifi_interface == NULL)
    {
        qDebug() << "wifi on failed event = " << eventWiFi;
        return -1;
    }
//    qDebug() << "\r\n\r\n\r\n\r\np_wifi_interface =   \r\n\r\n\r\n\r\n" << p_wifi_interface;

    // 等忙
    WiFi_waitBusy();

    return 0;

#endif


}// End of int DriverWiFi::WiFi_on(void)

/*******************************************************************************
*                           陆超@2016-09-30
* Function Name :   WiFi_off
* Description   :   关闭WiFi
* Input         :   None
* Output        :   None
* Return        :   -1 关闭失败 0 关闭成功
*******************************************************************************/
int DriverWiFi::WiFi_off(void)
{
#ifdef Bran_R8

    int iResult = -1;


    // 关Wi-Fi
    iResult = aw_wifi_off(p_wifi_interface);
    if(iResult < 0)
    {
        qDebug("Test failed: wifi off error!\n");
        return -1;
    }

    // wait event
    while(eventWiFi != WIFIMG_WIFI_OFF_SUCCESS)
    {
        usleep(200000);
        qDebug() << "wifi_off state busing, waiting\n";
    }

    qDebug("Wi-Fi关闭状态  = %s", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);

    return 0;

#endif


}// End of int DriverWiFi::WiFi_off(void)

/*******************************************************************************
*                           陆超@2016-10-11
* Function Name :   get_wifi_state
* Description   :   获取Wi-Fi状态
* Input         :   void
* Output        :   None
* Return        :   Wi-Fi状态
*******************************************************************************/
int DriverWiFi::get_wifi_state(void)
{

#ifdef Bran_R8

    int wifi_state = aw_wifi_get_wifi_state();

    qDebug("当前Wi-Fi状态  = %s", (char*)WiFi_stateTable[wifi_state - WIFI_STATE_OFFSET]);

    return (wifi_state);

#endif

}// End of int DriverWiFi::get_wifi_state(void)

/*******************************************************************************
*                           陆超@2016-10-17
* Function Name :   Get_Key_Type
* Description   :   获取加密类型
* Input         :   int key_mgmt　int型
* Output        :   None
* Return        :   tKEY_MGMT　型
*******************************************************************************/
tKEY_MGMT DriverWiFi::Get_Key_Type(int key_mgmt)
{
    tKEY_MGMT key_type = WIFIMG_NONE;

    // 转换加密方式类型
    switch(key_mgmt)
    {
        case WIFI_KEY_NONE:
        {
            key_type = WIFIMG_NONE;

        }break;
        case WIFI_KEY_WPA_PSK:
        {
            key_type = WIFIMG_WPA_PSK;

        }break;
        case WIFI_KEY_WPA2_PSK:
        {
            key_type = WIFIMG_WPA2_PSK;

        }break;
        case WIFI_KEY_WEP:
        {
            key_type = WIFIMG_WEP;

        }break;
        default:
            break;
    }

    return key_type;

}// End of tKEY_MGMT DriverWiFi::Get_Key_Type(int key_mgmt)

/*******************************************************************************
*                           陆超@2016-10-17
* Function Name :   WiFi_remove_network
* Description   :   删除某SSID
* Input         :   char *ssid  　待删除SSID名称
*                   int key_mgmt　加密类型
* Output        :   None
* Return        :   －１操作失败　０操作成功
*******************************************************************************/
int DriverWiFi::WiFi_remove_network(char *ssid, int key_mgmt)
{
#ifdef Bran_R8

    int iResult;

    tKEY_MGMT key_type = Get_Key_Type(key_mgmt);

    iResult = p_wifi_interface->remove_network(ssid, key_type);

    if(iResult == WIFI_MANAGER_SUCCESS)
    {
        qDebug("删除%s网络成功！", ssid);
    }
    else
    {
        qDebug("删除%s网络失败！", ssid);

    }

    // 等忙
    WiFi_waitBusy();

    return iResult;

#endif

}// End of int DriverWiFi::WiFi_remove_network(char *ssid, int key_mgmt)

/*******************************************************************************
*                           陆超@2016-10-1４
* Function Name :   WiFi_remove_all_networks
* Description   :   删除所有保存的SSID
* Input         :   None
* Output        :   None
* Return        :   －１操作失败　０操作成功
*******************************************************************************/
int DriverWiFi::WiFi_remove_all_networks(void)
{

#ifdef Bran_R8

    int iResult;


    iResult = p_wifi_interface->remove_all_networks();

    if(iResult == WIFI_MANAGER_SUCCESS)
    {
        qDebug("删除所有网络成功！");
    }
    else
    {
        qDebug("删除所有网络失败！");

    }

    // 等忙
    WiFi_waitBusy();

    return iResult;

#endif

}// End of int DriverWiFi::WiFi_remove_all_networks(void)


/*******************************************************************************
*                           陆超@2016-10-1４
* Function Name :   WiFi_listNetworks
* Description   :   列出存储的网络列表
* Input         :   char *p_networks    network缓存
*                   int   iSize         缓存大小
* Output        :   None
* Return        :   －１操作失败　０操作成功
*******************************************************************************/
int DriverWiFi::WiFi_listNetworks(char *p_networks, int iSize)
{

#ifdef Bran_R8
    int event_label;
    int iResult;
    int times = 0;
    do
    {
        times++;
        event_label = rand();

        iResult = p_wifi_interface->list_networks(p_networks, iSize, event_label);

        if(iResult == WIFI_MANAGER_SUCCESS)
        {
            qDebug("%s\n", p_networks);
            qDebug("******************************\n");
            qDebug("Wifi list networks test: Success!\n");
            qDebug("******************************\n");

            times = WIFI_CMD_RETRY;
        }
        else
        {
            qDebug( "list_networks failed! event_label = %s", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);

        }

        // 等忙
        WiFi_waitBusy();

    }while(times < WIFI_CMD_RETRY);

    return iResult;

#endif

}// End of int DriverWiFi::WiFi_listNetworks(char *p_networks, int iSize)

/*******************************************************************************
*                           陆超@2016-10-11
* Function Name :   WiFi_disconnectAP
* Description   :   断开AP
* Input         :   void
* Output        :   None
* Return        :   0成功 -1失败
*******************************************************************************/
int DriverWiFi::WiFi_disconnectAP(void)
{

#ifdef Bran_R8

    int iResult;
    int event_label;
    int wifi_state = WIFIMG_WIFI_DISABLED;
    wifi_state = get_wifi_state();
    if ( wifi_state == WIFIMG_WIFI_CONNECTED )
    {

        event_label = rand();
        iResult = p_wifi_interface->disconnect_ap( event_label );
        if (iResult != WIFI_MANAGER_SUCCESS)
        {
            // 输出错误信息
            qDebug( "call error! error eventWiFi = %s \n", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
            return -1;
        }
        else
        {
            qDebug( "disconnect ap ok! eventWiFi = %s \n", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
        }
    }

    // 等忙
    WiFi_waitBusy();

    return 0;

#endif

}// End of int DriverWiFi::WiFi_disconnectAP(void)

/*******************************************************************************
*                           陆超@2016-10-12
* Function Name :   WiFi_Get_MAC_IP
* Description   :   获取mac地址和IP
* Input         :   char *MAC   MAC缓存
*                   char *IP    IP缓存
* Output        :   None
* Return        :   -1 读取失败  0读取成功
*******************************************************************************/
int DriverWiFi::WiFi_Get_MAC_IP(char *MAC, char *IP)
{

#ifdef Bran_R8

    FILE *fstream = NULL;
    int iRead_len = 0;
    char buff[1024];
    char *p_readData;

    // 清缓存
    memset(buff, 0, sizeof(buff));


    // 读取MAC　IP 到文件流
    if((fstream = popen("ifconfig wlan0","r")) == NULL)
    {
        qDebug( "execute command failed: %s", strerror(errno));
        return -1;
    }


    // 提取文件流中数据
    iRead_len = fread(buff, 1, sizeof(buff), fstream);
    if(iRead_len)
    {

        // 提取MAC
        p_readData = NULL;
        p_readData = strstr(buff, WIFI_MAC_ADDR_SIGN);

        if (p_readData)
        {
            p_readData += sizeof(WIFI_MAC_ADDR_SIGN) - 1;
            memcpy(MAC, p_readData, 17);
            MAC[17] = '\0';
        }
        else
        {
            return -1;
        }

        // 提取IP
        p_readData = NULL;
        p_readData = strstr(buff, WIFI_IP_ADDR_SIGN);

        if (p_readData)
        {
            p_readData += sizeof(WIFI_IP_ADDR_SIGN) - 1;
            sscanf(p_readData, "%[^ ]", IP);
        }
        else
        {
            IP[0] = '\0';
            qDebug("未找到IP\n");
        }
        qDebug("读取的数据长度 = %d", iRead_len);
        qDebug("MAC = %s, IP = %s", MAC, IP);
        qDebug("%s",buff);
    }
    else
    {
        pclose(fstream);
        return -1;
    }

    pclose(fstream);

    return 0;

#endif

}// End of int DriverWiFi::WiFi_Get_MAC_IP(char *MAC, char *IP)


/*******************************************************************************
*                           陆超@2016-10-19
* Function Name :   WiFi_connectAP_withNetID
* Description   :   使用NetID 连接ａｐ
* Input         :   const char *net_id  ID字符串指针
* Output        :   None
* Return        :   -1 失败
*                   WIFIMG_NETWORK_CONNECTED
*                   WIFIMG_PASSWORD_FAILED
*                   WIFIMG_CONNECT_TIMEOUT
*******************************************************************************/
int DriverWiFi::WiFi_connectAP_withNetID(const char *net_id)
{
#ifdef Bran_R8
    int ret;
    int wifi_state = WIFIMG_WIFI_DISABLED;
    int event_label;
    int returnVal = -1;


    wifi_state = get_wifi_state();
    if ( (wifi_state == WIFIMG_WIFI_ENABLE)
      || (wifi_state == WIFIMG_WIFI_DISCONNECTED)
      || (wifi_state == WIFIMG_WIFI_CONNECTED) )
    {
        event_label = rand();

        // 连接AP
        qDebug( "start connect　AP with netID! netID = %s event_label = %d \n", net_id, event_label);
        ret = (p_wifi_interface->connect_ap_with_netid(net_id, event_label));
        if (ret == WIFI_MANAGER_SUCCESS )
        {
            qDebug( "Call connect ap with netID OK!\n" );

            if (WiFi_waitLabel(event_label) == 0)
            {

                qDebug( "connect ap with netID wait event! event_label = %d current event = %s\n", event_label, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);

                qDebug("connect ap with netID = %s eventWiFi = %s", net_id, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);

                if ((eventWiFi == WIFIMG_NETWORK_CONNECTED) || (eventWiFi == WIFIMG_CONNECT_TIMEOUT))
                {
                    returnVal = eventWiFi;
                }
                else
                {
                    while (1)
                    {
                        // 收到错误信息
                        qDebug( "connect ap with netID error ! error eventWiFi = %s \n", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
                        sleep(2);
                    }
                }
            }

        }
        else
        {
            // 参数和密码错都算密码错
            if (eventWiFi == WIFIMG_CMD_OR_PARAMS_ERROR)
            {
                eventWiFi = WIFIMG_PASSWORD_FAILED;
            }
            if (eventWiFi == WIFIMG_PASSWORD_FAILED)
            {
                returnVal = eventWiFi;

                qDebug("connect ap with netID = %s eventWiFi = %s", net_id, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
            }
            else
            {
                // 输出错误信息
                returnVal = -1;
                qDebug( "connectAP with NetID  call error!　netID = %s error eventWiFi = %s \n", net_id, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
            }
        }
    }
    else
    {
        returnVal = -1;
        qDebug( "WiFi isn't ready, waiting\n" );
    }

    WiFi_waitBusy();

    return (returnVal);

#endif


}// End of int DriverWiFi::WiFi_connectAP_withNetID(const char *net_id)

/*******************************************************************************
*                           陆超@2016-12-07
* Function Name :   WiFi_waitBusy
* Description   :   等忙
* Input         :   None
* Output        :   None
* Return        :   -1 失败　0 成功
*******************************************************************************/
int DriverWiFi::WiFi_waitBusy(void)
{
    int	times = 0;

    // 等待Wi-Fi空闲
    times = 0;
    do
    {

        if (get_wifi_state() != WIFIMG_WIFI_BUSING)
        {
            return 0;
        }
        else
        {
            qDebug() << "wifi_on state busing, waiting\n";
            usleep(WIFI_BUSY_SLEEP_TIME);
        }

    }while(times++ < WIFI_BUSY_TIMEOUT);

    return (-1);

}// End of int DriverWiFi::WiFi_waitBusy(void)

/*******************************************************************************
*                           陆超@2016-10-19
* Function Name :   WiFi_gitNetID
* Description   :   获取ID
* Input         :   char *SSID      SSID
*                   int keymgmt     加密方式
*                   char *netID     netID指针
* Output        :   None
* Return        :   0 存在该SSID  -1 不存在
*******************************************************************************/
int DriverWiFi::WiFi_gitNetID(char *SSID, int keymgmt, char *netID)
{
#ifdef Bran_R8
    int ret;
    int returnVal = -1;
    tKEY_MGMT key_type = Get_Key_Type(keymgmt);

    int times = 0;
    do
    {
        times++;

        // 获取netID
        ret = ( p_wifi_interface->get_netid(SSID, key_type, netID));

        if(ret == 0)
        {
            qDebug("-------------------------------The netid of your network is %s\n", netID);
            returnVal = 0;
        }
        else
        {
            qDebug("get netid failed!\n");
        }

        // 等忙
        WiFi_waitBusy();

    }while(times < WIFI_CMD_RETRY);

    return (returnVal);

#endif


}// End of int DriverWiFi::WiFi_gitNetID(char *SSID, int keymgmt, char *netID)

/*******************************************************************************
*                           陆超@2016-12-07
* Function Name :   WiFi_waitLabel
* Description   :   等同步信号
* Input         :   int event_label
* Output        :   None
* Return        :   -1 失败 0 成功
*******************************************************************************/
int DriverWiFi::WiFi_waitLabel(int event_label)
{
    int	times = 0;

    // 等待Wi-Fi空闲
    times = 0;
    do
    {

        if (callBack_label == event_label)
        {
            return 0;
        }
        else
        {
            qDebug() << " waiting callBack_label sync \n";
            usleep(WIFI_SLEEP_TIME);
        }

    }while(times++ < WIFI_WAIT_LABEL_TIMEOUT);

    return (-1);

}// End of int DriverWiFi::WiFi_waitLabel(int event_label)

/*******************************************************************************
*                           陆超@2016-11-01
* Function Name :   WiFi_addNetwork
* Description   :   连接匿名 AP
* Input         :   char *SSID      SSID
*                   char *key       密码
*                   int keymgmt     加密方式
* Output        :   None
* Return        :   -1 失败
*                   WIFIMG_NETWORK_CONNECTED
*                   WIFIMG_PASSWORD_FAILED
*                   WIFIMG_CONNECT_TIMEOUT
*******************************************************************************/
int DriverWiFi::WiFi_addNetwork(char *SSID, char *key, int keymgmt)
{
#ifdef Bran_R8
    int ret;
    int wifi_state = WIFIMG_WIFI_DISABLED;
    int returnVal = -1;
    int event_label;
    int key_len   = strlen(key);
    tKEY_MGMT key_type = Get_Key_Type(keymgmt);

    // 密码小于8位直接返回密码错误
    if ((key_len < 8) && (keymgmt != WIFI_KEY_NONE))
    {
        return WIFIMG_PASSWORD_FAILED;
    }
    // 读取wifi当前状态
    wifi_state = get_wifi_state();
    if ( (wifi_state == WIFIMG_WIFI_ENABLE)
      || (wifi_state == WIFIMG_WIFI_DISCONNECTED)
      || (wifi_state == WIFIMG_WIFI_CONNECTED) )
    {
        event_label = rand();

        // 连接AP
        qDebug( "start Wi-Fi add_network! event_label = %d \n", event_label);
        ret = (p_wifi_interface->add_network(SSID, key_type, key, event_label));
        if (ret == WIFI_MANAGER_SUCCESS )
        {

            qDebug( "Call Wi-Fi add_networkt OK!\n" );

            if (WiFi_waitLabel(event_label) == 0)
            {

                qDebug( "Wi-Fi add_network wait event! event_label = %d current event = %s\n", event_label, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
                qDebug( "Wi-Fi add_network SSID = %s event_label = %s", SSID, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
                if ((eventWiFi == WIFIMG_NETWORK_CONNECTED) || (eventWiFi == WIFIMG_CONNECT_TIMEOUT))
                {
                    returnVal = eventWiFi;
                }
                else
                {
                    while (1)
                    {
                        // 收到错误信息
                        qDebug( "Wi-Fi add_network error ! error eventWiFi = %s \n", (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
                        sleep(2);
                    }
                }
            }

        }
        else
        {
            // 参数和密码错都算密码错
            if (eventWiFi == WIFIMG_CMD_OR_PARAMS_ERROR)
            {
                eventWiFi = WIFIMG_PASSWORD_FAILED;
            }
            if (eventWiFi == WIFIMG_PASSWORD_FAILED)
            {
                returnVal = eventWiFi;

                qDebug( "Wi-Fi add_network SSID = %s = %s", SSID, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
            }
            else
            {
                // 输出错误信息
                returnVal = -1;
                qDebug( "Wi-Fi add_network call error!　connect SSID = %s error eventWiFi = %s \n", SSID, (char*)WiFi_statusTable[eventWiFi - WIFI_STATUS_OFFSET]);
            }
        }
    }
    else
    {
        returnVal = -1;
        qDebug( "WiFi isn't ready, waiting\n" );
    }

    // 等忙
    WiFi_waitBusy();

    return (returnVal);

#endif


}// End of int DriverWiFi::WiFi_addNetwork(char *SSID, char *key, int keymgmt)

/********************************* END OF FILE ********************************/

