#ifndef SYSOCC1_H
#define SYSOCC1_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include "math.h"

class SysOCC1 : public QThread
{

    Q_OBJECT
public:
    SysOCC1();
    void stop();
    void set_start();

protected:
    void run();


private:

    bool isFor;

};

#endif // SYSOCC1_H
