#ifndef SYSOCC2_H
#define SYSOCC2_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include "math.h"

class SysOCC2 : public QThread
{

    Q_OBJECT
public:
    SysOCC2();
    void stop();
    void set_start();

protected:
    void run();


private:

    bool isFor;
};

#endif // SYSOCC2_H
