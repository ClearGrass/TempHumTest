/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WIFIDATAOBJECT_H
#define WIFIDATAOBJECT_H

#include <QObject>
#include "sysWiFi.h"

class WifiDataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ssid READ ssid WRITE set_ssid NOTIFY signal_ssidChanged)
    Q_PROPERTY(int signalNum READ signalNum WRITE set_signalNum NOTIFY signal_signalNumChanged)
    Q_PROPERTY(int status READ status WRITE set_status NOTIFY signal_statusChanged)
    Q_PROPERTY(bool lock READ lock WRITE set_lock NOTIFY signal_lockChanged)
    Q_PROPERTY(int strength READ strength WRITE set_strength NOTIFY signal_strengthChanged)
    Q_PROPERTY(QString ip READ ip WRITE set_ip NOTIFY signal_ipChanged)
    Q_PROPERTY(bool key READ key WRITE set_key NOTIFY signal_keyChanged)

public:
    WifiDataObject(QObject *parent=0);
    WifiDataObject(WiFi wifi, QObject *parent=0);


    void set_ssid(const QString &ssid);
    QString ssid();

    int signalNum();
    void set_signalNum(const int signalNum);

    int status();
    void set_status(const int status);

    bool  lock();
    void set_lock(const bool lock);

    int strength();
    void set_strength(const int strength);

    QString ip();
    void set_ip(const QString ip);

    bool key();
    void set_key(const bool key);


signals:
    void signal_ssidChanged();
    void signal_signalNumChanged();
    void signal_statusChanged();
    void signal_lockChanged();
    void signal_strengthChanged();
    void signal_ipChanged();
    void signal_keyChanged();

private:
    QString sSsid;                                                       // Wi-Fi名称
    int iSignalNum;                                                      // 信号格数
    int iStatus;                                                         // 当前状态
    bool bLock;                                                          // 是否加密
    int iStrength;                                                       // 信号强度
    QString sIp;                                                         // IP地址
    bool iKey;
};

#endif // WIFIDATAOBJECT_H
