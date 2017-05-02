#include "historyPage.h"
#include "ui_historyPage.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <qmath.h>
#include "time.h"


HistoryPage::HistoryPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryPage)
{
    ui->setupUi(this);

    switchControl = new SwitchControl(this);
    switchControl->resize(234, 56);
    switchControl->move(580, 22);
    funcCity        = FuncCity::getInstance();

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    this->move(0, 0);
    isHours =  true;
    sysControl = SysControl::getInstance();



    //初始化7天单选框组
    listLabel.push_back(ui->X1);
    listLabel.push_back(ui->X2);
    listLabel.push_back(ui->X3);
    listLabel.push_back(ui->X4);
    listLabel.push_back(ui->X5);
    listLabel.push_back(ui->X6);
    listLabel.push_back(ui->X7);



    //7天位置信息
    QList<QRect> rectList;


    rectList.push_back(getAbsoluteRect(ui->line_1->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_2->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_3->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_4->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_5->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_6->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_7->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_8->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_9->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_10->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_11->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_12->geometry()));
    rectList.push_back(getAbsoluteRect(ui->line_13->geometry()));



    //7天和24小时页面对象定义
    historyChart7Days   = new HistoryChartBase(0,this, Page_7Days);
    historyChart24Hours = new HistoryChartBase(0,this,Page_24Hours);


    historyChart7Days->set_labels(ui->value_hours, ui->value_max, ui->value_min);
    historyChart24Hours->set_labels(ui->value_hours, ui->value_max, ui->value_min);

    //设置7天位置信息
    historyChart7Days->set_xRectList(rectList);
    historyChart24Hours->set_xRectList(rectList);

    historyChart7Days->hide();
    historyChart24Hours->show();

    connect(switchControl, SIGNAL(toggled(bool)), this, SLOT(slot_switchControlClicked(bool)));
    connect(historyChart7Days, SIGNAL(signal_unavailable(bool)), this, SLOT(slot_unavailable(bool)));
    connect(historyChart24Hours, SIGNAL(signal_unavailable(bool)), this, SLOT(slot_unavailable(bool)));


    weatherHours = new WeatherChartBase(this, Page_24Hours);
    weatherDays = new WeatherChartBase(this, Page_7Days);
    weatherHours->raise();
    weatherDays->raise();
    weatherHours->hide();
    weatherDays->hide();

}
/*******************************************************************************
* Function Name :   ~SetWifiPage
* Description   :   析构函数
* Input         :   None
* Output        :   回收对象
* Return        :   None
*******************************************************************************/
HistoryPage::~HistoryPage(void)
{
    delete ui;
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_index
* Description   :   设置当前页面
* Input         :   int index ：页面index
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryPage::set_index(int index)
{
    switchControl->setToggle(true);
    currentPage = index;

    if(WEATHRT_PAGE != index)
    {
        set_labelVisible(true);

        ui->frame_days->setVisible(false);
        ui->frame_hours->setVisible(true);

        //改变子对象当前页面
        historyChart24Hours->change_index(index);
        historyChart7Days->change_index(index);

        //刷新数据模型
        historyChart7Days->init_data();
        historyChart24Hours->init_data();
        historyChart7Days->hide();
        historyChart24Hours->show();
        weatherHours->hide();
        weatherDays->hide();


        init_coordinateAxis();
        set_unit(index);
    }
    else
    {
        weatherDays->move(0, weatherDays->y());
        weatherHours->move(0, weatherHours->y());
        set_labelVisible(false);
        ui->label_dataName->setText(funcCity->get_location());
        historyChart7Days->hide();
        historyChart24Hours->hide();
        weatherHours->show();
        weatherDays->hide();
        weatherHours->refresh_data();
        weatherDays->refresh_data();
    }
}



/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   on_button_back_clicked
* Description   :   触发返回信号
* Input         :   None
* Output        :   触发signal_backDefaultPage信号
* Return        :   None
*******************************************************************************/
void HistoryPage::on_button_back_clicked()
{
    emit signal_backDefaultPage();
}

void HistoryPage::slot_switchControlClicked(bool flag)
{
    isHours = flag;
    if(WEATHRT_PAGE == currentPage)
    {
        if(flag)
        {
            weatherHours->show();
            weatherDays->hide();
        }
        else
        {
            weatherHours->hide();
            weatherDays->show();
        }
        return ;
    }

    set_labelVisible(true);

    if(flag)
    {
        ui->frame_days->setVisible(false);
        ui->frame_hours->setVisible(true);
        historyChart7Days->hide();
        historyChart24Hours->show();
    }
    else
    {
        ui->frame_days->setVisible(true);
        ui->frame_hours->setVisible(false);
        historyChart7Days->show();
        historyChart24Hours->hide();
    }
    init_coordinateAxis();
}

void HistoryPage::slot_unavailable(bool visible)
{
    set_labelVisible(!visible);
}

QRect HistoryPage::getAbsoluteRect(QRect rect)
{
    return QRect(rect.x() , rect.y()  - 102, rect.width(), rect.height());
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   init_coordinateAxis
* Description   :   初始化坐标轴
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryPage::init_coordinateAxis()
{
    QString strTmp  = "hh:00";

    if(switchControl->isHoursChecked())
    {
        //时间格式化字符串 默认24小时制
        QString timeFormat = "hh:00";

        //如果系统为12小时制，则修改格式化字符串
        if(sysControl->get_value(TIME_MODE) != DEFAULT_TIME_MODE)
        {
            timeFormat = "hh AP";
        }
        strTmp= tr("Now");
        ui->X1->setText(strTmp);

        QTime time = QTime::currentTime().addSecs(60 * 60);

        for(int i = 1; i < 7; i++)
        {

            listLabel[i]->setText(time.addSecs(-2 * i * 60 * 60).toString(QString("%1").arg(timeFormat)));
        }
    }
    else
    {
        QDate date = QDate::currentDate();
        strTmp = tr("Today");
        listLabel[0]->setText(strTmp);
        for(int i = 1; i < 7; i++)
        {
            listLabel[i]->setText(date.addDays(-i).toString("MM/dd"));
        }
    }

}

void HistoryPage::set_unit(int index)
{


    QString strUnit = "";
    QString strDataName = "";
    switch (index) {
    case PM_PAGE:
        strUnit = "μg/m³";
        strDataName= tr("PM2.5 History");
        break;
    case TEMP_PAGE:
        if(sysControl->get_value(TEMP_UNIT) == "cel")
        {
            strUnit = "℃";
        }
        else
        {
            strUnit = "℉";
        }
        strDataName= tr("Temp History");

        break;
    case HUM_PAGE:
        strUnit ="%";
        strDataName= tr("RH History");
        break;
    case CO2_PAGE:
        strUnit ="ppm";
        strDataName= tr("CO2e History");

        break;
    case TVOC_PAGE:
        strUnit = sysControl->get_value(TVOC_UNIT);
        strDataName= tr("tVOC History");

        if(strUnit == "mg/m3")
        {
            strUnit = "mg/m³";
        }
        break;
    default:

        break;
    }

    ui->unit->setText(strUnit);
    ui->unit_1->setText(strUnit);
    ui->unit_2->setText(strUnit);
    ui->label_dataName->setText(strDataName);
}

void HistoryPage::set_labelVisible(bool isVisible)
{
    ui->X1->setVisible(isVisible);
    ui->X2->setVisible(isVisible);
    ui->X3->setVisible(isVisible);
    ui->X4->setVisible(isVisible);
    ui->X5->setVisible(isVisible);
    ui->X6->setVisible(isVisible);
    ui->X7->setVisible(isVisible);
    ui->frame_scale->setVisible(isVisible);

    ui->frame_days->setVisible(isVisible);
    ui->frame_hours->setVisible(isVisible);

    ui->unit->setVisible(isVisible);
    ui->value_max->clear();
    ui->value_min->clear();
    ui->value_hours->clear();

}

void HistoryPage::mousePressEvent(QMouseEvent *event)
{
    if(WEATHRT_PAGE != currentPage)
    {
        return ;
    }
    prevPoint = event->pos();

}

void HistoryPage::mouseMoveEvent(QMouseEvent *event)
{
    if(WEATHRT_PAGE != currentPage)
    {
        return ;
    }
    int space = event->pos().x() - prevPoint.x();
    prevPoint = event->pos();
    move_subWigget(space);
}

void HistoryPage::mouseReleaseEvent(QMouseEvent *event)
{
    if(WEATHRT_PAGE != currentPage)
    {
        return ;
    }
    int space = event->pos().x() - prevPoint.x();
    prevPoint = event->pos();
    move_subWigget(space);

}

void HistoryPage::move_subWigget(int space)
{
    if(switchControl->isHoursChecked())
    {

        QPoint postmp = weatherHours->pos();
        if(postmp.x() + space <= -(WEATHER_HOURS_PAGE_WIDTH- 854) )
        {
            weatherHours->move(-(WEATHER_HOURS_PAGE_WIDTH- 854), postmp.y());
        }
        else if(postmp.x() + space >= 0)
        {
            weatherHours->move(0, postmp.y());
        }
        else
        {
            weatherHours->move(postmp.x() + space, postmp.y());
        }
    }
    else
    {
        QPoint postmp = weatherDays->pos();


        if(postmp.x() + space <= -(WEATHER_DAYS_PAGE_WIDTH- 854) )
        {
            weatherDays->move(-(WEATHER_DAYS_PAGE_WIDTH- 854), postmp.y());
        }
        else if(postmp.x() + space >= 0)
        {
            weatherDays->move(0, postmp.y());
        }
        else
        {
            weatherDays->move(postmp.x() + space, postmp.y());
        }
    }
}


