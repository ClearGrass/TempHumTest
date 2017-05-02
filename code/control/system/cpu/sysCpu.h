#ifndef SYSCPU_H
#define SYSCPU_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include "math.h"

class SysCPU : public QThread
{
    Q_OBJECT
public:
    SysCPU();

signals:
    void signal_update_cpuFre(float);
    void signal_update_cpuOcc(QString);

private slots:
    void slot_change_fre(bool);
    void slot_change_occ(bool);
    void slot_update_data();

protected:
    void run();

private:
    void increase_frequency();
    void reduce_frequency();
    void increase_occ();
    void reduce_occ();

private:
    QTimer *timerUpdate;

    bool isFor;

};

#endif // SYSCPU_H
