#include <QApplication>
#include <QTranslator>
#include <QTextStream>
#include <iostream>
#include <QWSServer>
#include <QDebug>
#include "control/mainControl.h"
#include "miio/miioControl.h"

/*******************************************************************************
* Function Name :   customMessagehandler
* Description   :   输出信息重定向
* Input         :   QtMsgType type:输出信息类型
*                   const char *msg:输出信息
* Output        :   初始化
* Return        :   None
*******************************************************************************/
void customMessagehandler(QtMsgType type, const char *msg)
{
    QString txt;
    QString curDateTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (type) {
    case QtDebugMsg:

        //调试信息
        txt = QString("Debug: %1:%2").arg(curDateTime).arg(msg);
        break;
    case QtWarningMsg:

        //警告信息
        txt = QString("Warning: %1:%2").arg(curDateTime).arg(msg);
        break;
    case QtCriticalMsg:

        //临界信息
        txt = QString("Critical: %1:%2").arg(curDateTime).arg(msg);
        break;
    case QtFatalMsg:

        //致命信息
        txt = QString("Fatal: %1%2").arg(curDateTime).arg(msg);
        break;
    default:
        break;
    }

    //日志文件
    QFile outFile(LOGPATH);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);

    //打印信息到终端
    std::cout << txt.toStdString() << endl;
    //输出重定向
    ts << txt << endl;


}

int main(int argc, char *argv[])
{

#ifdef Bran_R8
    //设置背景为黑色
    QWSServer::setBackground(QBrush(QColor(0, 0, 0,0)));

    //设置输出重定向
    //    qInstallMsgHandler(customMessagehandler);
#endif
    QApplication a(argc, argv);

#ifdef Bran_R8

    // 鼠标隐藏
    QWSServer::setCursorVisible(false);
#endif

    MainControl *mainControl = new MainControl();
    return a.exec();
}


