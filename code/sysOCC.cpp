#include "sysOCC.h"

SysOCC::SysOCC()
    :QThread()
{
}

void SysOCC::run()
{
    isFor = true;
    float a,b,c,d,e;
    while(true)
    {
        if(!isFor)
        {
            qDebug()<<"66666666666666666666666666666";
            break;

        }
        else
        {
            a=13243;
            for(int i =0;i < 100000; ++i)
            {
                b =sin(a);
                c=sin(a/b);
                d=asin(c);//h弧度
                e=(d*360)/(2*3.14);
                a= e;
                c=sin(a/b);
                d=asin(c);//h弧度
                e=(d*360)/(2*3.14);
                c=sin(a/b);
                d=asin(c);//h弧度
                e=(d*360)/(2*3.14);
            }
        }
        this->msleep(1);
    }

    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
}

void SysOCC::stop()
{
    isFor = false;
    qDebug()<<"################################################################";
}

