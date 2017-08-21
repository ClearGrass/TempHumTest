#include "sysCpu.h"

SysCPU::SysCPU()
    : QThread()
{
    timerUpdate = new QTimer();
    timerUpdate->setInterval(1000);
    timerUpdate->moveToThread(this);
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slot_update_data()));
    system(qPrintable("echo Userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"));
    system(qPrintable("ls -t /usr/bin/qtapp/debugFile/ | awk '{if(NR >5) {print \"rm /usr/bin/qtapp/debugFile/\"$0}}' | sh"));
    increase_frequency();

    moveToThread(this);
}

void SysCPU::run()
{
    timerUpdate->start();
    this->exec();
}

void SysCPU::slot_update_data()
{
    float occ;
    FILE  *stream1;
    char  buf1[4096];
    QString strtmp1;

    // 获取cpu占用数据
    stream1 = popen("top -b -n 1 | grep BranQt4 | grep -v grep", "r");
    fread( buf1, sizeof(char), sizeof(buf1),  stream1);
    strtmp1 = buf1;
    strtmp1 = strtmp1.simplified();
    QStringList cpuList = strtmp1.split(" ");
    qDebug()<<cpuList.length()<<strtmp1;
    if(cpuList.length() >= 6)
    {
        occ = cpuList[8].toFloat();
        qDebug()<<occ;
    }
    emit signal_update_cpuOcc(QString("%1").arg(occ));
    pclose(stream1);
}

void SysCPU::slot_change_fre(bool type)
{
    if(type)
    {
        // 主频调到1.08G
        increase_frequency();
    }
    else
    {
        // 主频调到120M
        reduce_frequency();
    }

}

void SysCPU::slot_change_occ(bool type)
{
    if(type)
    {
        //　降低CPU频率
        reduce_occ();
    }
    else
    {
        // 将CPU占用调整至100%
        increase_occ();
    }
}


/*******************************************************************************
* Function Name  :  slot_frequency_increase
* Description    :  增加频率
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysCPU::increase_frequency()
{
    FILE  *stream;
    char  buf[4096];;
    QString str;
    float fre;

    // 将此时的频率提升到1.008
    system(qPrintable("echo 1008000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"));

    // 获取cpu主频
    stream = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    str = buf;
    str = QString::number((str.toLong())/1000000.0);
    fre = str.toFloat();
    emit signal_update_cpuFre(fre);
    pclose(stream);
}

/*******************************************************************************
* Function Name  :  slot_frequency_reduce
* Description    :  降低频率
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysCPU::reduce_frequency()
{
    FILE  *stream;
    char  buf[4096];;
    QString str;
    float fre;

    // 将此时的频率降低至120
    system(qPrintable("echo 120000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"));

    // 获取cpu主频
    stream = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    str = buf;
    str = QString::number((str.toLong())/1000000.0);
    fre = str.toFloat();
    emit signal_update_cpuFre(fre);
    pclose(stream);
}

void SysCPU::increase_occ()
{
    int i = 0;

    isFor = true;
    float a,b,c,d,e;
    while(isFor)
    {
        a=1;
        b=2;
        c=a/b;
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);
    }
    //    system(qPrintable("for i in `seq 1 $(cat /proc/cpuinfo |grep \"physical id\" |wc -l)`; do dd if=/dev/zero of=/dev/null & done"));
}

void SysCPU::reduce_occ()
{
    isFor = false;
    //    system(qPrintable("pkill -9 dd"));
}

