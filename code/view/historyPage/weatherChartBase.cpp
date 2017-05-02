#include "weatherChartBase.h"
#include <QtDebug>
#include <QPushButton>
//默认坐标轴范围定义
Range WeatherChartBase::rangeDefalut[] = { 350,200,35,20,55,40,850,500,3,0 };

WeatherChartBase::WeatherChartBase(QWidget *parent,Page_Type currentPage) :
    QWidget(parent)
{

    //设置当前页面
    this->currentPage = currentPage;
    CG = new ColorGenerator();
    CG->setIndex(WEATHRT_PAGE);
    //设置页面大小

    if(currentPage == Page_7Days)
    {
        this->setGeometry(0,102,WEATHER_DAYS_PAGE_WIDTH,378);
    }
    else
    {
        this->setGeometry(0,102,WEATHER_HOURS_PAGE_WIDTH,378);
    }

    waitWeatherData = new Waiting(this);
    messageBox = new QLabel(this);
    messageBox->setStyleSheet("font: 26px \"MI LANTING\";\n    color:#A9AAAA;");
    messageBox->resize(854, 378);
    messageBox->move(0, 0);
    messageBox->setAlignment(Qt::AlignCenter);
    messageBox->hide();
    waitWeatherData->move(854/2-waitWeatherData->width()/2, 480 / 2 + waitWeatherData->height() / 2 - 102);
    //初始化成员变量
    bPaintInit = false;
    m_7DaysRectList.clear();
    sysControl = SysControl::getInstance();
    funcWeather = FuncWeather::getInstance();
    isUsLang = sysControl->get_value(LANGUAGE).contains("US");

    connect(funcWeather, SIGNAL(signal_update_weatherDaily(QList<WeatherDaily> *)), this, SLOT(slot_update_weatherDaily(QList<WeatherDaily> *)));
    connect(funcWeather, SIGNAL(signal_update_weatherHourly(QList<WeatherHourly> *)), this, SLOT(slot_update_weatherHourly(QList<WeatherHourly> *)));
    connect(funcWeather, SIGNAL(signal_net_abnormal()), this, SLOT(slot_net_abnormal()));
    connect(funcWeather, SIGNAL(signal_net_disconnect()), this, SLOT(slot_net_disconnect()));
    connect(this, SIGNAL(signal_get_weather()), funcWeather, SLOT(slot_request_weather()));

}



/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   paintEvent
* Description   :   重绘事件
* Input         :   QPaintEvent 绘制事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::paintEvent(QPaintEvent *)
{
    if(!bPaintInit)
    {
        return ;
    }
    //字体定义
    QFont font;
    font.setFamily("MI LANTING");
    font.setPixelSize(24);

    QPainter wp(this);
    wp.setRenderHint(QPainter::Antialiasing, true);

    //画时间轴
    draw_times(&wp);

    //12小时
    if(currentPage != Page_24Hours)
    {
        draw_maxAndMin(&wp);
    }

    draw_points(&wp);

    if(currentPage == Page_24Hours)
    {
        draw_weatherIcon(&wp);
    }



}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   init_range
* Description   :   坐标轴值域初始化  只针对12小时数据
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::init_range()
{
    //临时变量
    QList<float> listValue;
    if(currentPage == Page_24Hours)
    {
        for(int loop = 0; loop < 12; ++loop)
        {
            listValue.append(listWeatherHours.at(loop).temp);
        }

        init_range(listValue);


    }
    else
    {

        for(int i = 0; i < 7; ++i)
        {

            listValue.append(listWeatherDaily.at(i).tempMax);
            listValue.append(listWeatherDaily.at(i).tempMin);
        }

        init_range(listValue);

    }
    return;
}

void WeatherChartBase::init_range(QList<float> &listValue)
{
    //排序
    qSort(listValue.begin(),listValue.end());

    //获取最小值
    while(listValue.size() >0)
    {
        iMinRange = listValue.first();
        if(iMinRange != ERROR_DATA)
        {
            break;
        }
        listValue.pop_front();
    }

    //获取最大值
    while(listValue.size() >0)
    {
        iMaxRange = listValue.last();
        if(iMaxRange != ERROR_DATA)
        {
            break;
        }
        listValue.pop_back();
    }


    if(iMaxRange ==ERROR_DATA || iMinRange == ERROR_DATA )
    {
        //如果库中数据为空，则设置默认的范围
        iMaxRange = rangeDefalut[WEATHRT_PAGE].maxRange;
        iMinRange = rangeDefalut[WEATHRT_PAGE].minRange;
    }
}


void WeatherChartBase::draw_maxAndMin(QPainter *painter)
{
    //画笔设置
    painter->setPen(QPen(Qt::white));

    QFont font;
    font.setFamily(FONT_FAMILY);
    font.setPixelSize(24);
    painter->setFont(font);
    QString strUnit = "";
    if(sysControl->get_value(TEMP_UNIT) == "cel")
    {
        strUnit = "℃";

    }
    else
    {
        strUnit = "℉";
    }


    for(int i = 0; i < 7; ++i)
    {
        //天气图标

        painter->drawImage(QRectF(X0 + i * X_SPACE -ICON_SIZE / 2, DAYS_Y_TOP, ICON_SIZE, ICON_SIZE),
                           QImage(QString(":/resource/images/weather/icon/%1.png").arg(listWeatherDaily[i].skycon)));

        //最大值
        painter->drawText(QRectF(X0 + i * X_SPACE - DAYS_TEXT_WIDRH / 2, DAYS_VALUE_Y_TOP, DAYS_TEXT_WIDRH, DAYS_TEXT_HEIGHT),
                          Qt::AlignCenter,QString("%1%2").arg(listWeatherDaily[i].tempMax).arg(strUnit));

        //最小值
        painter->drawText(QRectF(X0 + i * X_SPACE - DAYS_TEXT_WIDRH / 2, DAYS_VALUE_Y_BOTTOM,DAYS_TEXT_WIDRH, DAYS_TEXT_HEIGHT ),
                          Qt::AlignCenter,QString("%1%2").arg(listWeatherDaily[i].tempMin).arg(strUnit));
    }


}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_times
* Description   :   时间轴绘制
* Input         :   QPainter *painter：绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::draw_times(QPainter *painter)
{

    //时间格式化字符串 默认24小时制
    QString timeFormat = "hh:00";

    QFont font;
    font.setFamily(FONT_FAMILY);
    font.setPixelSize(20);

    QPen pen(Qt::white);
    painter->setFont(font);
    painter->setRenderHint(QPainter::Antialiasing, true);

    //当前日期时间
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString strText = "";
    if(currentPage == Page_24Hours)
    {

        //如果系统为12小时制，则修改格式化字符串
        if(sysControl->get_value(TIME_MODE) != DEFAULT_TIME_MODE)
        {
            timeFormat = "hh AP";
        }

        for(int i = 0; i < 12; i++)
        {
            QRect rect(i * X_SPACE, 328, X_SPACE, 50);
            if(i == 0)
            {
                strText = tr("Now");
                pen.setColor(Qt::white);
            }
            else
            {
                strText = curDateTime.time().addSecs(i * 60 * 60).toString(timeFormat);
                pen.setColor(QColor("#A9AAAA"));
            }
            painter->setPen(pen);
            painter->drawText(rect,Qt::AlignCenter,strText);

        }
    }
    else
    {
        //英文
        if(isUsLang)
        {
            timeFormat = "MM/dd";
        }
        else
        {
            timeFormat = "MM月dd日";
        }

        for(int i = 0; i < 7; i++)
        {
            QRect recttmp(i * X_SPACE, 328, X_SPACE, 50);

            if(i == 0)
            {
                pen.setColor(QColor("#A9AAAA"));
                strText = tr("Yesterday");
            }
            else if(i == 1)
            {
                pen.setColor(Qt::white);
                strText = tr("Today");
            }
            else if(i == 2)
            {
                pen.setColor(QColor("#A9AAAA"));
                strText = tr("Tomorrow");
            }
            else
            {
                pen.setColor(QColor("#A9AAAA"));
                strText =  curDateTime.date().addDays(i - 1).toString(timeFormat);
            }
            painter->setPen(pen);
            painter->drawText(recttmp,Qt::AlignCenter,strText);
        }
    }

    pen.setColor(QColor("#A9AAAA"));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(0, 328, this->width(), 328);

}

void WeatherChartBase::draw_weatherIcon(QPainter *painter)
{

    QPen pen = QPen(Qt::white,1, Qt::DashLine, Qt::SquareCap);

    pen.setColor(QColor("#A9AAAA"));

    painter->setPen(pen);
    int leftIndex = 0;
    int i = 0;
    for(; i < listWeatherHours.size() - 1;i++)
    {
        if(listWeatherHours[i].skycon.trimmed() != listWeatherHours[i + 1].skycon.trimmed())
        {

            if(i - leftIndex <= 1)
            {
                painter->drawImage(QRect(X0 + leftIndex * X_SPACE - 15, HOURS_Y_BOTTOM + 20, 31,31),
                                   QImage(QString(":/resource/images/weather/icon/%1.png").arg(listWeatherHours[i].skycon)));
                pen.setColor(Qt::white);

                painter->drawText(QRect(leftIndex * X_SPACE, HOURS_Y_BOTTOM + 61, X_SPACE, DAYS_TEXT_HEIGHT),Qt::AlignCenter,get_weatherDescribe(listWeatherHours[i].skycon));

            }
            else
            {

                pen.setColor(QColor("#A9AAAA"));
                painter->drawLine(listPoint12Hours[leftIndex].posX ,listPoint12Hours[leftIndex].posY + 8,listPoint12Hours[leftIndex].posX, 328);
                int x = listPoint12Hours[leftIndex].posX + (listPoint12Hours[i].posX - listPoint12Hours[leftIndex].posX) / 2;
                painter->drawImage(QRect(x - 15, HOURS_Y_BOTTOM + 20, 31,31),
                                   QImage(QString(":/resource/images/weather/icon/%1.png").arg(listWeatherHours[i].skycon)));
                pen.setColor(Qt::white);
                painter->drawText(QRect(x - X_SPACE, HOURS_Y_BOTTOM + 61, X_SPACE * 2, DAYS_TEXT_HEIGHT),Qt::AlignCenter,get_weatherDescribe(listWeatherHours[i].skycon));
            }

            if(i <= 1)
                leftIndex = i+ 1;
            else
                leftIndex = i;


        }

    }
    pen.setColor(QColor("#A9AAAA"));
    painter->drawLine(listPoint12Hours[leftIndex].posX ,listPoint12Hours[leftIndex].posY + 8,listPoint12Hours[leftIndex].posX, 328);
    painter->drawLine(listPoint12Hours[i].posX ,listPoint12Hours[i].posY + 8,listPoint12Hours[i].posX, 328);

    int x = listPoint12Hours[leftIndex].posX + (listPoint12Hours[i].posX - listPoint12Hours[leftIndex].posX) / 2;
    painter->drawImage(QRect(x - 15, HOURS_Y_BOTTOM + 20, 31,31),
                       QImage(QString(":/resource/images/weather/icon/%1.png").arg(listWeatherHours[i].skycon)));
    pen.setColor(Qt::white);
    painter->drawText(QRect(x - X_SPACE, HOURS_Y_BOTTOM + 61, X_SPACE * 2, DAYS_TEXT_HEIGHT),Qt::AlignCenter,get_weatherDescribe(listWeatherHours[i].skycon));


}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   init_points
* Description   :   点初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::init_points()
{
    //当前页面为24小时
    if( currentPage == Page_24Hours)
    {
        listPoint12Hours.clear();
        for(int j = 0; j < 12; j++)
        {
            Point newPoint;
            newPoint.value = listWeatherHours[j].temp;

            float i = ((newPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(HOURS_Y_BOTTOM - HOURS_Y_TOP);
            newPoint.posX = X0 + j * X_SPACE;
            //最大值和最小值相等时
            if(iMaxRange == iMinRange)
            {
                //坐标为1/2个Y值域
                newPoint.posY = (HOURS_Y_BOTTOM - HOURS_Y_TOP) / 2 + HOURS_Y_TOP;
            }
            else
            {
                newPoint.posY = HOURS_Y_BOTTOM -i;
            }
            listPoint12Hours.push_back(newPoint);

        }
    }
    else
    {
        listPoint7Days[0].clear();
        listPoint7Days[1].clear();
        for(int j = 0; j < 7 ; j++)
        {
            Point maxPoint;

            maxPoint.value =  listWeatherDaily[j].tempMax ;

            maxPoint.posX =  X0 + j * X_SPACE;

            if(iMaxRange == iMinRange)
            {
                maxPoint.posY = (DAYS_LINE_Y_BOTTOM - DAYS_LINE_Y_TOP) / 2 + DAYS_LINE_Y_TOP;
            }
            else
            {
                maxPoint.posY = DAYS_LINE_Y_BOTTOM -((maxPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(DAYS_LINE_Y_BOTTOM - DAYS_LINE_Y_TOP);
            }

            listPoint7Days[0].append(maxPoint);

            Point minPoint;
            minPoint.value = listWeatherDaily[j].tempMin;
            minPoint.posX = X0 + j * X_SPACE;

            if(iMinRange == iMaxRange)
            {
                minPoint.posY = (DAYS_LINE_Y_BOTTOM - DAYS_LINE_Y_TOP) / 2 + DAYS_LINE_Y_TOP;
            }
            else
            {
                minPoint.posY = DAYS_LINE_Y_BOTTOM - ((minPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(DAYS_LINE_Y_BOTTOM - DAYS_LINE_Y_TOP);
            }
            listPoint7Days[1].append(minPoint);

        }
    }
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_points
* Description   :   画点
* Input         :   QPainter *painter：绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::draw_points(QPainter *painter)
{
    init_points();

    //反锯齿设置
    painter->setRenderHint(QPainter::Antialiasing, true);
    if( currentPage == Page_24Hours )
    {
        if(listPoint12Hours.size() <=0 )
        {
            return ;
        }
        draw_points(painter,listPoint12Hours);
    }
    else
    {
        draw_points(painter,listPoint7Days[0]);
        draw_points(painter,listPoint7Days[1]);
    }

}


void WeatherChartBase::draw_points(QPainter *painter, QList<Point> &pointList)
{

    QFont font;
    font.setFamily(FONT_FAMILY);
    font.setPixelSize(24);
    QString strUnit;
    if( currentPage == Page_24Hours)
    {
        if(sysControl->get_value(TEMP_UNIT) == "cel")
        {
            strUnit = "℃";

        }
        else
        {
            strUnit = "℉";
        }
    }

    QBrush brush;
    brush.setColor(QColor("#292A2C"));
    QPen penLine, penEllipse;
    penLine.setWidth(2);
    penEllipse.setWidth(2);
    if( currentPage != Page_24Hours)
    {
        penLine.setColor(QColor("#808080"));
        penEllipse.setColor(Qt::white);
    }
    else
    {
        penLine.setColor(QColor("#33b4ff"));
        penEllipse.setColor(QColor("#33b4ff"));
    }
    for(int i = 1; i < pointList.size(); i++)
    {

        float value1 = pointList[i].value;
        float value2 = pointList[i-1].value;
        //获取连续的两个点坐标
        QPoint startPos = QPoint(pointList[i-1].posX , pointList[i-1].posY);;
        QPoint endPos(pointList[i].posX, pointList[i].posY);

        if(value2 != ERROR_DATA && value1 != ERROR_DATA)
        {
            int y1, y2;
            if( currentPage == Page_24Hours )
            {
                if(endPos.y() > startPos.y())
                {
                    y1 =  startPos.y() + 2;
                    y2 = endPos.y() -2;
                }
                else if(endPos.y() < startPos.y())
                {
                    y1 =  startPos.y() - 2;
                    y2 = endPos.y() +2;
                }
                else
                {
                    y1 =  startPos.y();
                    y2 = endPos.y() ;
                }
            }
            else
            {
                y1 =  startPos.y();
                y2 = endPos.y() ;
            }
            painter->setPen(penLine);
            painter->setBrush(brush);
            painter->drawLine(startPos.x() + 13, y1, endPos.x() - 13, y2 );
        }

        if(value2 != ERROR_DATA)
        {
            painter->setPen(penEllipse);
            painter->setBrush(brush);

            painter->drawEllipse(startPos.x()-8, startPos.y()-8, 16, 16);

            if( currentPage == Page_24Hours)
            {
                painter->setPen(QPen(Qt::white, 2));
                painter->setFont(font);

                painter->drawText(QRect(startPos.x(), startPos.y()-54,DAYS_TEXT_WIDRH,DAYS_TEXT_HEIGHT),Qt::AlignLeft|Qt::AlignBottom,QString("%1%2").arg(pointList[i-1].value).arg(strUnit));
            }
        }
        //绘制最后一个点
        if(i == pointList.size()-1 && value1 != ERROR_DATA)
        {
            painter->setPen(penEllipse);
            painter->setBrush(brush);
            painter->drawEllipse(endPos.x()-8, endPos.y()-8, 16, 16);
            if( currentPage == Page_24Hours)
            {
                painter->setPen(QPen(Qt::white, 2));
                painter->setFont(font);
                painter->drawText(QRect(endPos.x(), endPos.y()-54,DAYS_TEXT_WIDRH,DAYS_TEXT_HEIGHT),Qt::AlignHCenter|Qt::AlignBottom,QString("%1%2").arg(pointList[i].value).arg(strUnit));
            }
        }

    }
}

void WeatherChartBase::refresh_data()
{
    bPaintInit = false;
    messageBox->hide();
    waitWeatherData->startAnimation();

    emit signal_get_weather();
}

QString WeatherChartBase::get_weatherDescribe(QString des)
{
    QString strTmp = "";
    if(des == "CLEAR_DAY")
    {
        strTmp = tr("Sunny");
    }
    else if (des == "CLEAR_NIGHT")
    {
        strTmp = tr("Clear");

    }
    else if ((des == "PARTLY_CLOUDY_DAY")|| (des == "PARTLY_CLOUDY_NIGHT"))
    {
        strTmp = tr("Partly Cloudy");
    }
    else if(des == "CLOUDY")
    {
        strTmp = tr("Cloudy");
    }
    else if(des == "RAIN")
    {
        strTmp = tr("Rain");
    }
    else if(des == "SNOW")
    {
        strTmp = tr("Snow");
    }
    else if(des == "WIND")
    {
        strTmp = tr("Windy");
    }
    else if(des == "FOG")
    {
        strTmp = tr("Foggy");
    }
    else if(des == "HAZE")
    {
        strTmp = tr("Haze");
    }
    else if(des == "SLEET")
    {
        strTmp = tr("Sleet");
    }
    return strTmp;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_paintInit
* Description   :   设置页面数据重绘
* Input         :   bool flag:
*                   true:重绘
*                   false:不重绘
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::set_paintInit(bool flag)
{
    bPaintInit = flag;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_7daysRect
* Description   :   设置7天位置信息
* Input         :   QList<QRect> rectList  位置列表
* Output        :   None
* Return        :   None
*******************************************************************************/
void WeatherChartBase::set_7daysRect(QList<QRect> rectList)
{
    m_7DaysRectList = rectList;

}




void WeatherChartBase::slot_update_weatherDaily(QList<WeatherDaily> *weatherDaily)
{
    if( currentPage == Page_24Hours )
    {
        return ;
    }
    listWeatherDaily.clear();

    for(int i = weatherDaily->length() -1; i >= 0; --i)
    {
        listWeatherDaily.push_front(weatherDaily->at(i));
    }

    init_range();
    messageBox->hide();
    bPaintInit = true;
    update();
    waitWeatherData->stopAnimation();
}

void WeatherChartBase::slot_update_weatherHourly(QList<WeatherHourly> *weatherHours)
{
    if( currentPage == Page_7Days )
    {
        return ;
    }
    listWeatherHours.clear();
    for(int i = weatherHours->length() - 1; i >= 0; --i)
    {
        listWeatherHours.push_front(weatherHours->at(i));
    }

    init_range();
    messageBox->hide();
    bPaintInit = true;
    update();
    waitWeatherData->stopAnimation();

}

void WeatherChartBase::slot_net_abnormal()
{
    listWeatherDaily.clear();
    listWeatherHours.clear();
    waitWeatherData->stopAnimation();
    messageBox->setText(tr("Internet Abnormal"));
    messageBox->show();
}

void WeatherChartBase::slot_net_disconnect()
{
    listWeatherDaily.clear();
    listWeatherHours.clear();

    messageBox->setText(tr("Not Connected"));
    waitWeatherData->stopAnimation();
    messageBox->show();
}
