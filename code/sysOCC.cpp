#include "sysOCC.h"

SysOCC::SysOCC()
    :QThread()
{
    isFor = false;
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
            qDebug() << "2222222222222--------------或或或";
            qDebug() << "3333333333333--------------或或或";
            qDebug() << "444444444444--------------或或或";

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

void SysOCC::testFunc1()
{
    float a,b,c,d,e;

    for(int i =0;i < 99999; ++i)
    {
        int cc[1024];
        b =sin(a);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
        a= e;
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);

    }
}
void SysOCC::testFunc2()
{
    float a,b,c,d,e;

    for(int i =0;i < 99999; ++i)
    {
        int cc[1024];

        b =sin(a);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
        a= e;
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);

    }
}
void SysOCC::testFunc3()
{
    float a,b,c,d,e;

    for(int i =0;i < 99999; ++i)
    {
        int cc[1024];
        b =sin(a);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
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
        e=(d*360)/(2*3.14)*i;
        a= e;
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);
        c=sin(a/b);
        d=asin(c);//h弧度
        e=(d*360)/(2*3.14);

    }
}

void SysOCC::stop()
{
    isFor = false;
    qDebug()<<"66666666666666666666666666666";
    qDebug() << "5555555555555--------------或或或";
    qDebug() << "66666--------------或或或";
    qDebug() << "77777--------------或或或";
}

void SysOCC::set_start()
{
    qDebug() << "11111111111111111111111111111111111111111111111";
    qDebug() << "22222222222222222222222222222222222222222222222";
    qDebug() << "333333333333333333333333333333333333333333333";

    isFor = true;
    this->start();
}

