#ifndef SYSOCC_H
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

protected:
    void run();

private:

    bool isFor;

};

#endif // SYSOCC_H
