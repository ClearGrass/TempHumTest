﻿#ifndef SYSOCC_H
#define SYSOCC_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include "math.h"

class SysOCC : public QThread
{

    Q_OBJECT
public:
    SysOCC();
    void stop();
    void set_start();

protected:
    void run();
    void testFunc1();
    void testFunc2();
    void testFunc3();

private:

    bool isFor;

};

#endif // SYSOCC_H
