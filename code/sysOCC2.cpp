#include "sysOCC2.h"

SysOCC2::SysOCC2()
    :QThread()
{
    isFor = false;
}

void SysOCC2::run()
{
    isFor = true;
    float a,b,c,d,e;
    while(true)
    {
        if(!isFor)
        {

            break;

        }
        else
        {
            a=13243;
            for(int i =0;i < 999999; ++i)
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
    //    this->exec();
}


void SysOCC2::stop()
{
    isFor = false;

}

void SysOCC2::set_start()
{


    isFor = true;
    this->start();
}

