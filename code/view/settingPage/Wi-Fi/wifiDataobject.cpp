#include <QDebug>
#include "wifiDataobject.h"

WifiDataObject::WifiDataObject(QObject *parent)
    : QObject(parent)
{
}

WifiDataObject::WifiDataObject(WiFi wifi, QObject *parent)
    : QObject(parent), sSsid(wifi.ssid),iSignalNum(wifi.signalNum),iStatus(wifi.status),bLock(wifi.lock),
      iStrength(wifi.strength),sIp(wifi.ip),iKey(wifi.key)
{
}


void WifiDataObject::set_ssid(const QString &ssid)
{
    if(ssid != sSsid)
    {
        sSsid = ssid;
        emit signal_ssidChanged();
    }
}

QString WifiDataObject::ssid()
{
    return sSsid;
}

int WifiDataObject::signalNum()
{
    return iSignalNum;
}

void WifiDataObject::set_signalNum(const int signalNum)
{
    if(iSignalNum != signalNum)
    {
        iSignalNum = signalNum;
        emit signal_signalNumChanged();
    }
}

int WifiDataObject::status()
{
    return iStatus;
}

void WifiDataObject::set_status(const int status)
{
    if(status != iStatus)
    {
        iStatus = status;
        emit signal_statusChanged();
    }
}

bool WifiDataObject::lock()
{
    return bLock;
}

void WifiDataObject::set_lock(const bool lock)
{
    if(lock != bLock)
    {
        bLock = lock;
        emit signal_lockChanged();
    }
}

int WifiDataObject::strength()
{
    return  iStrength;
}

void WifiDataObject::set_strength(const int strength)
{
    if(iStrength != strength)
    {
        iStrength = strength;
        emit signal_strengthChanged();
    }
}

QString WifiDataObject::ip()
{
    return sIp;
}

void WifiDataObject::set_ip(const QString ip)
{
    if(sIp != ip)
    {
        sIp = ip;
        emit signal_ipChanged();
    }
}

bool WifiDataObject::key()
{
    return iKey;
}

void WifiDataObject::set_key(const bool key)
{
    if(key != iKey)
    {
        iKey = key;
        emit signal_keyChanged();
    }
}

