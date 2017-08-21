#include "mainControl.h"
PageType gCurrent_pageIndex = PAGE_MAIN;
MainControl::MainControl(QObject *parent)
    : QGraphicsView()
{
    //设置场景
    mainScene = new QGraphicsScene;
    this->setScene(mainScene);
    style_init();

    // 系统初始化
    system_init();
    data_init();


    widget_init();

    if("true" == fileConfig->get_value(SYS_ENTER))
    {
        fileConfig->set_value(SYS_ENTER, "false");
    }

    // 若在底座上，直接显示出主界面，数据采集开始
    dataAirControl->start();
    sysWiFi->start();
    pageMainProxy->show();
    gCurrent_pageIndex = PAGE_MAIN;




    //为场景对象添加事件过滤
    //    mainScene->installEventFilter(this);
    this->show();
}

/*******************************************************************************
* Function Name  :  style_init
* Description    :  风格初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::style_init()
{
    mainScene->setSceneRect(QRect(0,0,854,480));

    this->setGeometry(QRect(0,0,854,480));
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background: transparent;border:0px");
}


/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::data_init()
{
    bAllowDrop = false;
    isScreenOn = true;
    mouseType = Release;
    timerPowerOff = new QTimer();
    timerPowerOff->setInterval(POWER_OFF_INTERVAL);
    timerPowerOff->setSingleShot(true);

    connect(timerPowerOff, SIGNAL(timeout()), this, SLOT(slot_device_powerOff()));
    connect(timerPowerOff, SIGNAL(timeout()), sysControl, SLOT(slot_device_powerOff()));

    timerScreenOff = new QTimer();
    timerScreenOff->setInterval(SCREEN_OFF_INTERVAL);
    timerScreenOff->setSingleShot(true);
    connect(timerScreenOff, SIGNAL(timeout()), sysControl, SLOT(slot_screenOff()));



}

/*******************************************************************************
* Function Name  :  widget_init
* Description    :  界面初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::widget_init()
{
    mainViewQml = new MainViewQML();
    pageMainProxy = mainScene->addWidget(mainViewQml);
    pageMainProxy->setZValue(0);
    pageMainProxy->hide();

    pageSetting = new PageSetting();
    pageSettingProxy = mainScene->addWidget(pageSetting);
    pageSettingProxy->setZValue(1);
    pageSettingProxy->hide();



    // 关机选择界面
    pagePowerOff = new PagePowerOff();
    pagePowerOffProxy = mainScene->addWidget(qobject_cast<QWidget *>(pagePowerOff));
    pagePowerOffProxy->setZValue(10);
    pagePowerOff->hide();


    connect(sysControl, SIGNAL(signal_button_singleClick()), this, SLOT(slot_button_singleClick()));
    connect(this, SIGNAL(signal_back_page()), pageSetting, SLOT(slot_back()));
    connect(this, SIGNAL(signal_change_page()), mainViewQml, SLOT(slot_sysbuttonClick()));
    connect(this, SIGNAL(signal_mouseMove()), mainViewQml, SLOT(slot_mouseMove()));
    connect(this, SIGNAL(signal_marginTouch(bool)), mainViewQml, SLOT(slot_mouseMarginTouch(bool)));


    connect(mainViewQml,SIGNAL(signal_jumpOptionPage()),this,SLOT(slot_jump_pageSetting()));
    connect(pageSetting,SIGNAL(signal_jumpMainPage()),this,SLOT(slot_jump_pageMain()));
    connect(mainViewQml, SIGNAL(signal_jumpWiFiPage()), pageSetting, SLOT(slot_showWifiPage()));
    connect(mainViewQml, SIGNAL(signal_jumpAppPage()), pageSetting, SLOT(slot_showMiAppPage()));
    connect(mainViewQml, SIGNAL(signal_device_powerOff()), sysControl, SLOT(slot_device_powerOff()));
    connect(pageSetting, SIGNAL(signal_device_reboot()), sysControl, SLOT(slot_device_reboot()));

    connect(pageSetting,SIGNAL(signal_timeModeChange()),mainViewQml,SLOT(slot_timeModeChange()));
    connect(pageSetting,SIGNAL(signal_update_firmware()),mainViewQml,SLOT(slot_update_firmware()));
    connect(pageSetting,SIGNAL(signal_updateAppBindStatus(int)),mainViewQml,SLOT(slot_updateAppBindstat(int)));

    //Wi-Fi与主页面通讯信号绑定
    connect(sysWiFi, SIGNAL(signal_update_wifi(WiFi)), mainViewQml, SLOT(slot_update_wifi(WiFi)));


    connect(mainViewQml, SIGNAL(signal_horizonScreen()), pagePowerOff, SLOT(slot_changeHorizonScreen()));
    connect(mainViewQml, SIGNAL(signal_vertialScreen()), pagePowerOff, SLOT(slot_changevertialScreen()));

    //    connect(mainViewQml, SIGNAL(signal_setMatrix(QMatrix,DIRECTION)), this, SLOT(slot_setMatrix(QMatrix, DIRECTION)));


    connect(pagePowerOff, SIGNAL(signal_device_powerOff()), sysControl, SLOT(slot_device_powerOff()));
    connect(pagePowerOff, SIGNAL(signal_device_screenOff()), sysControl, SLOT(slot_screenOff()));
    connect(pagePowerOff, SIGNAL(signal_device_powerOff()), this, SLOT(slot_device_powerOff()));
    connect(sysControl, SIGNAL(signal_button_longPress()), this, SLOT(slot_button_longPress()));

    dropDownQml = new DropDown();
    dropDownProxy = mainScene->addWidget(dropDownQml);
    dropDownProxy->setZValue(2);
    dropDownQml->move(0, -480);

    dropDownAnimation = new QPropertyAnimation(dropDownQml, "pos");
    dropDownAnimation->setDuration(300);

    dropUpAnimation = new QPropertyAnimation(dropDownQml, "pos");
    dropUpAnimation->setDuration(300);


    connect(dropDownQml, SIGNAL(signal_device_screenOff()), sysControl, SLOT(slot_screenOff()));
    connect(dropDownQml, SIGNAL(signal_dropUp()), this, SLOT(slot_dropUp()));

}

/*******************************************************************************
* Function Name  :  system_init
* Description    :  系统初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::system_init()
{
    database        = Database::getInstance();
    sysControl      = SysControl::getInstance();
    wilddogControl  = WilddogControl::getInstance();
    serverControl   = ServerControl::getInstance();
    dataAirControl  = DataAirControl::getInstance();
    debugControl    = DebugControl::getInstance();

    sysWiFi         = SysWiFi::getInstance();
    fileConfig      = FileConfig::getInstance();

    funcApp         = FuncApp::getInstance();
    funcWeather     = FuncWeather::getInstance();
    funcCity        = FuncCity::getInstance();
    funcFirmware    = FuncFirmware::getInstance();

    connect(sysControl, SIGNAL(signal_net_connected()), funcApp, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), funcApp, SLOT(slot_net_disconnect()));
    connect(sysControl, SIGNAL(signal_net_connected()), funcFirmware, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), funcFirmware, SLOT(slot_net_disconnected()));
    connect(sysControl, SIGNAL(signal_net_connected()), funcCity, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), funcCity, SLOT(slot_net_disconnect()));
    connect(sysControl, SIGNAL(signal_net_connected()), funcWeather, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), funcWeather, SLOT(slot_net_disconnect()));
    connect(funcCity, SIGNAL(signal_update_cityID(QString)), funcWeather, SLOT(slot_update_cityID(QString)));

    connect(sysControl, SIGNAL(signal_net_connected()), wilddogControl, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), wilddogControl, SLOT(slot_net_disconnect()));
    connect(sysControl, SIGNAL(signal_net_connected()), serverControl, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), serverControl, SLOT(slot_net_disconnect()));
    connect(sysControl, SIGNAL(signal_battery_charging(bool)),dataAirControl, SLOT(slot_battery_charging(bool)));
    //    connect(wilddogControl, SIGNAL(signal_verify_net()), sysWiFi, SLOT(slot_net_abnormal()));
    connect(sysControl, SIGNAL(signal_power_off()), this, SLOT(slot_device_powerOff()));
    connect(this, SIGNAL(signal_interface_operations()), sysControl, SLOT(slot_interface_operations()));
    connect(sysControl, SIGNAL(signal_screen_on()), this, SLOT(slot_screen_on()));
    connect(sysControl, SIGNAL(signal_screen_off()), this, SLOT(slot_screen_off()));
    connect(funcFirmware, SIGNAL(signal_device_reboot()), sysControl, SLOT(slot_device_reboot()));
    connect(funcApp, SIGNAL(signal_create_qrcode(QString)), this, SLOT(slot_create_qrcode(QString)));
    connect(this, SIGNAL(signal_createQRCode_result(int)), funcApp, SLOT(slot_createQRCode_result(int)));

    // 基本系统开始运行
    sysControl->start();
    //    funcFirmware->start();
//    funcCity->start();
    //    funcWeather->start();
    //    funcApp->start();
    //    database->start();
//    serverControl->start();
    //    wilddogControl->start();
    //    debugControl->start();
}

/*******************************************************************************
* Function Name  :  slot_enter_system
* Description    :  初始化系统
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_init_system()
{

    pageWizardProxy->hide();
    pageInit = new WizardWait();
    pageInitProxy = mainScene->addWidget(pageInit);
    pageInitProxy->setZValue(10);
    pageInitProxy->show();

    //延迟100毫秒，让初始化等待页面显示出来
    QEventLoop loop;
    QTimer::singleShot(30, &loop, SLOT(quit()));
    loop.exec();

    qDebug()<<"INIT SYSTEM";
    dataAirControl->start();
    widget_init();
    pageMainProxy->show();
    pageInitProxy->hide();
    gCurrent_pageIndex = PAGE_MAIN;
}

/*******************************************************************************
* Function Name  :  slot_enter_system
* Description    :  进入系统
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_enter_system()
{
    qDebug()<<"ENTER SYSTEM";
    timerPowerOff->stop();
    timerScreenOff->stop();
    dataAirControl->start();
    sysWiFi->start();
    pagePowerOn->hide();
    pageMainProxy->show();
    gCurrent_pageIndex = PAGE_MAIN;
}

/*******************************************************************************
* Function Name  :  slot_button_longPress
* Description    :  长按按钮
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_button_longPress()
{
    pagePowerOff->show_page();
}

/*******************************************************************************
* Function Name  :  slot_button_singleClick
* Description    :  按钮单击
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_button_singleClick()
{
    // 若是主界面则更换界面
    if(PAGE_MAIN == gCurrent_pageIndex || PAGE_HIS == gCurrent_pageIndex )
    {
        emit signal_change_page();
    }
    // 若是设置页面则返回上一层
    else if(PAGE_SETTING == gCurrent_pageIndex)
    {
        emit signal_back_page();
    }
}




/*******************************************************************************
* Function Name  :  slot_jump_pageMain
* Description    :  跳转到主界面
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_jump_pageMain()
{
    pageSettingProxy->hide();
    pageMainProxy->show();
    gCurrent_pageIndex = PAGE_MAIN;
}

/*******************************************************************************
* Function Name  :  slot_jump_pageSetting
* Description    :  跳转到设置页面
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_jump_pageSetting()
{
    pageMainProxy->hide();
    pageSettingProxy->show();
    gCurrent_pageIndex = PAGE_SETTING;
}

/*******************************************************************************
* Function Name  :  slot_device_powerOff
* Description    :  播放关机动画
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_device_powerOff()
{
    qDebug()<<"POWER OFF";
    QDeclarativeView *animalPowerOff = new QDeclarativeView(this);

    //设置透明
    animalPowerOff->setAttribute(Qt::WA_TranslucentBackground, true);
    animalPowerOff->setSource(QUrl(QML_POWEROFF_SOURCE_URL));
    animalPowerOff->show();
}

/*******************************************************************************
* Function Name  :  slot_screen_on
* Description    :  界面亮起
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_screen_on()
{
    if(PAGE_POWER_ON == gCurrent_pageIndex)
    {
        timerScreenOff->start();
    }
    isScreenOn = true;
}

/*******************************************************************************
* Function Name  :  slot_screen_off
* Description    :  熄屏
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_screen_off()
{
    isScreenOn = false;
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-12
* Function Name :   slot_setMatrix
* Description   :   旋转
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainControl::slot_setMatrix(QMatrix matrix,  DIRECTION direction)
{

    currentDirection = direction;
    if(direction == VERTICAL)
    {
        dropDownQml->hide();
        pagePowerOff->setGeometry(QRect(0,-187,480,854));
        pagePowerOff->rotate(90);
    }
    else
    {
        dropDownQml->show();
        pagePowerOff->setGeometry(QRect(0,0,854,480));
        pagePowerOff->rotate(-90);
    }
    qDebug() << "slot_setMatrix,===================== " << direction ;
    this->setMatrix(matrix);

}

/*******************************************************************************
* Function Name :   eventFilter
* Description   :   QObject *object, QEvent *event
* Input         :   QObject *object 接受事件对象
*                   QEvent *event   所发生的事件
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainControl::eventFilter(QObject *object, QEvent *event)
{
    //只过滤当前场景
    if(object == mainScene)
    {
        if((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::GraphicsSceneMousePress))
        {
            if(event->type() == QEvent::GraphicsSceneMouseRelease)
            {
                mouseType = Release;
            }
            else
            {
                mouseType = Press;
            }

            emit signal_interface_operations();
        }
        if(event->type() == QEvent::GraphicsSceneMouseMove  && mouseType == Press)
        {
            emit signal_mouseMove();
        }

        if((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::GraphicsSceneMousePress)
                || (event->type() == QEvent::GraphicsSceneMouseMove))
        {
            if(QCursor::pos().y() < 15  || (QCursor::pos().y() > 465 && dropDownQml->y() == 0))
            {
                bAllowDrop = true;

                if(QCursor::pos().y() < 50)
                    emit signal_marginTouch(true);
            }

            if((event->type() != QEvent::GraphicsSceneMouseRelease))
            {
                if(bAllowDrop)
                {
                    dropDownAnimation->setStartValue(dropDownQml->pos());
                    dropDownAnimation->setEndValue(QPoint(0, QCursor::pos().y() - 480));
                    dropDownAnimation->setDuration(50);
                    dropDownAnimation->start();
                }
            }
            else
            {
                if(bAllowDrop)
                {

                    if(dropDownQml->y() > -240)
                    {
                        dropDownAnimation->setStartValue(dropDownQml->pos());
                        dropDownAnimation->setEndValue(QPoint(0, 0));
                        dropDownAnimation->setDuration(150);
                        dropDownAnimation->start();
                        bAllowDrop = false;

                    }
                    else
                    {
                        dropUpAnimation->setStartValue(dropDownQml->pos());
                        dropUpAnimation->setEndValue(QPoint(0,-480));
                        dropUpAnimation->start();
                        bAllowDrop = false;
                        emit signal_marginTouch(false);
                    }
                }
            }
        }
    }
    return false;
}


/*******************************************************************************
* Function Name  :  slot_create_qrcode
* Description    :  创建二维码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainControl::slot_create_qrcode(QString data)
{
    int width = 320;
    int height = 320;
    QPixmap pixmap(width,height);
    QPainter painter(&pixmap);
    QRcode *qr = QRcode_encodeString(data.toStdString().c_str(), 1, QR_ECLEVEL_H, QR_MODE_8, 1);
    if(0 != qr)
    {
        QColor fg("black");
        QColor bg("white");
        painter.setBrush(bg);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0,0,width,height);
        painter.setBrush(fg);
        const int s=qr->width>0?qr->width:1;
        const double w=width-20;
        const double h=height-20;
        const double aspect=w/h;
        const double scale=((aspect>1.0)?h:w)/s;
        for(int y=0;y<s;y++)
        {
            const int yy=y*s;
            for(int x=0;x<s;x++)
            {
                const int xx=yy+x;
                const unsigned char b=qr->data[xx];
                if(b &0x01)
                {
                    const double rx1=x*scale+10, ry1=y*scale+10;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r,1);
                }
            }
        }
        QPixmap logo(":/resource/images/optionPage/label/logo_qrcode.png");
        int logoWidth = width*0.2;
        int logoHeight = height * 0.2;
        painter.drawPixmap(width/2-logoWidth/2,height/2-logoHeight/2,logoWidth,logoHeight,logo);
        QRcode_free(qr);
    }
    else{
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0,0,width,height);
    }
    qr=0;
    pixmap.save(QRCODE_PATH,"png");
    emit signal_createQRCode_result(0);
}

void MainControl::slot_dropUp()
{
    dropUpAnimation->setStartValue(dropDownQml->pos());
    dropUpAnimation->setEndValue(QPoint(0,-480));
    dropUpAnimation->start();
    bAllowDrop = false;
    emit signal_marginTouch(false);
}

