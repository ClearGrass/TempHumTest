#include "historyChartBase.h"
#include <QtDebug>
#include <QPushButton>
//默认坐标轴范围定义
Range HistoryChartBase::rangeDefalut[] = { 350,200,35,20,55,40,850,500,3,0 };

HistoryChartBase::HistoryChartBase(int index,QWidget *parent,Page_Type currentPage) :
    QWidget(parent)
{

    //设置当前页面
    this->currentPage = currentPage;

    //设置页面大小
    this->setGeometry(0,102,854,378);

    dataAirControl = DataAirControl::getInstance();

    //设置当前页面index
    iCurrentIndex = index;
    waitWeatherData = new Waiting(this);
    messageBox = new QLabel(this);
    messageBox->setStyleSheet("font: 26px \"MI LANTING\";\n    color:#A9AAAA;");
    messageBox->resize(this->size());
    messageBox->move(0, 0);
    messageBox->setAlignment(Qt::AlignCenter);
    messageBox->hide();
    waitWeatherData->move(854/2-waitWeatherData->width()/2, 378 / 2 + waitWeatherData->height() / 2 - 102);
    //初始化成员变量
    touchDay = 0;
    touchPoint.posX = -100;
    bPaintInit = false;
    isTouch = false;
    xRectList.clear();
    sysControl = SysControl::getInstance();
    isUsLang = sysControl->get_value(LANGUAGE).contains("US");

    connect(dataAirControl, SIGNAL(signal_update_historyDaily(Data7Days)), this, SLOT(slot_update_historyDaily(Data7Days)));
    connect(dataAirControl, SIGNAL(signal_update_historyHourly(Data12Hours)), this, SLOT(slot_update_historyHourly(Data12Hours)));
    connect(this, SIGNAL(signal_get_historyDaily(AirDataType)), dataAirControl, SLOT(slot_get_historyDaily(AirDataType)));
    connect(this, SIGNAL(signal_get_historyHourly(AirDataType)), dataAirControl, SLOT(slot_get_historyHourly(AirDataType)));
}



/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   paintEvent
* Description   :   重绘事件
* Input         :   QPaintEvent 绘制事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::paintEvent(QPaintEvent *)
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

    //判断数据是否为空
    if(isEmpty())
    {

        messageBox->setText(tr("Historical data unavailable"));
        waitWeatherData->stopAnimation();
        messageBox->show();

        emit signal_unavailable(true);
        return ;
    }



    //12小时
    if(currentPage == Page_24Hours)
    {

        //设置渐变色对象 当前页面
        init_pen();

        wp.setFont(font);

        //反锯齿 设置
        wp.setRenderHint(QPainter::Antialiasing, true);


        //画坐标轴
        draw_range(&wp);

        //画时间轴
        draw_times(&wp);

        wp.setFont(font);
        //画点
        draw_points(&wp);



        //设置反锯齿
        wp.setRenderHint(QPainter::Antialiasing, true);


    }
    else
    {


        init_pen();


        init_range();

        wp.setFont(font);

        //反锯齿 设置
        wp.setRenderHint(QPainter::Antialiasing, true);

        draw_points(&wp);

    }

    //画点击label
    draw_label(&wp);


}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   init_range
* Description   :   坐标轴值域初始化  只针对12小时数据
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::init_range()
{
    //临时变量
    QList<float> listValue;
    if(currentPage == Page_24Hours)
    {
        for(int loop = 0; loop < 12; ++loop)
        {

            listValue.append(dataHours.nearestValue[loop]);
        }

        init_range(listValue);


    }
    else
    {

        for(int i = 0; i < 7; ++i)
        {

            listValue.append(dataDays.maxValue[i]);
            listValue.append(dataDays.minValue[i]);
        }

        init_range(listValue);

    }
    return;
}

void HistoryChartBase::init_range(QList<float> &listValue)
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
        iMaxRange = rangeDefalut[iCurrentIndex].maxRange;
        iMinRange = rangeDefalut[iCurrentIndex].minRange;
    }
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_range
* Description   :   坐标轴绘制
* Input         :   QPainter *painter:绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::draw_range(QPainter *painter)
{
    //坐标轴值域初始化
    init_range();
}



/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_times
* Description   :   时间轴绘制
* Input         :   QPainter *painter：绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::draw_times(QPainter *painter)
{

    //时间格式化字符串 默认24小时制
    QString timeFormat = "hh:00";

    //如果系统为12小时制，则修改格式化字符串
    if(sysControl->get_value(TIME_MODE) != DEFAULT_TIME_MODE)
    {
        timeFormat = "hh AP";
    }
    listPoint12Hours.clear();

    QDateTime curTime = QDateTime::currentDateTime();

    for (int i = 0 ; i < TOTAL_HOURS; ++i)
    {
        //x坐标计算

        Point p;

        if(i == 0)
        {
            p.time = tr("Now");
        }
        else
        {
            p.time = curTime.addSecs(-(i*60*60)).toString(QString("%1").arg(timeFormat));
        }

        listPoint12Hours.push_back(p);

    }

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   init_points
* Description   :   点初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::init_points()
{
    //当前页面为24小时
    if( currentPage == Page_24Hours)
    {

        for(int j = 0; j < 12; j++)
        {
            Point newPoint;


            newPoint.value = dataHours.nearestValue[j];

            float i = ((newPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(BOTTOM_Y - TOP_Y);

            if(j < 1)
            {
                newPoint.posX = xRectList.at(j).x() ;
            }
            else
            {
                newPoint.posX = xRectList.at(j + 1 ).x() ;
            }

            //最大值和最小值相等时
            if(iMaxRange == iMinRange)
            {
                //坐标为1/2个Y值域
                newPoint.posY = (BOTTOM_Y - TOP_Y) / 2 ;
            }
            else
            {
                newPoint.posY = BOTTOM_Y  -i;
            }
            listPoint12Hours[j].value = newPoint.value;
            listPoint12Hours[j].posX = newPoint.posX;
            listPoint12Hours[j].posY = newPoint.posY;

        }
    }
    else
    {
        listPoint7Days[0].clear();
        listPoint7Days[1].clear();
        for(int j = 0; j < 7 ; j++)
        {
            Point maxPoint;

            maxPoint.value =  dataDays.maxValue[j];
            maxPoint.posX = xRectList[j * 2].x() ;
            float tmp = ((maxPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(BOTTOM_Y - TOP_Y);

            if(iMaxRange == iMinRange)
            {
                maxPoint.posY = (BOTTOM_Y - TOP_Y) / 2;
            }
            else
            {
                maxPoint.posY = BOTTOM_Y  -tmp;
            }

            listPoint7Days[0].append(maxPoint);

            Point minPoint;
            minPoint.value = dataDays.minValue[j];
            minPoint.posX = xRectList[j * 2].x();
            tmp = ((minPoint.value - iMinRange)*1.0/(iMaxRange - iMinRange))*(BOTTOM_Y - TOP_Y);
            if(iMinRange == iMaxRange)
            {
                minPoint.posY =(BOTTOM_Y - TOP_Y) / 2;
            }
            else
            {
                minPoint.posY = BOTTOM_Y  -tmp;
            }
            listPoint7Days[1].append(minPoint);

        }
    }
}

void HistoryChartBase::init_data()
{
    get_daysData();
    get_hoursData();
    messageBox->hide();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_points
* Description   :   画点
* Input         :   QPainter *painter：绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::draw_points(QPainter *painter)
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
        painter->setPen(QPen(QBrush(QColor("#23a6de")), 6,  Qt::SolidLine,  Qt::RoundCap));
        draw_points(painter,listPoint12Hours);
    }
    else
    {
        painter->setPen(QPen(QBrush(QColor("#23a6de")), 6,  Qt::SolidLine,  Qt::RoundCap));
        draw_line_points(painter,listPoint7Days[0]);
        painter->setPen(QPen(QBrush(QColor("#29d7c1")), 6,  Qt::SolidLine,  Qt::RoundCap));
        draw_line_points(painter,listPoint7Days[1]);
    }

}

void HistoryChartBase::draw_points(QPainter *painter, QList<Point> &pointList)
{

    int index = 0;
    while(pointList[index].value == ERROR_DATA)
    {
        index++;
        if(index >= pointList.size())
        {
            return ;
        }
    }
    QPainterPath Path(QPoint(pointList[index].posX, pointList[index].posY));

    for(int i = 0; i < pointList.size() - 1 ; i++)
    {

        float value1 = pointList[i].value;
        float value2 = pointList[i+1].value;
        //获取连续的两个点坐标
        QPoint startPos = QPoint(pointList[i].posX, pointList[i].posY);
        QPoint endPos(pointList[i + 1].posX, pointList[i + 1].posY);


        if(value2 != ERROR_DATA && value1 != ERROR_DATA)
        {
            painter->setBrush(Qt::NoBrush);
            QPoint c1((startPos.x()+endPos.x())/2,startPos.y());
            QPoint c2((startPos.x()+endPos.x())/2,endPos.y());
            Path.cubicTo(c1 ,c2 ,endPos );
            painter->drawPath(Path);

        }

    }
}

void HistoryChartBase::draw_line_points(QPainter *painter, QList<Point> &pointList)
{
    for(int i = 1; i < pointList.size(); i++)
    {

        float value1 = pointList[i].value;
        float value2 = pointList[i-1].value;

        //获取连续的两个点坐标
        QPoint startPos = QPoint(pointList[i-1].posX , pointList[i-1].posY);;
        QPoint endPos(pointList[i].posX, pointList[i].posY);

        if(value2 != ERROR_DATA && value1 != ERROR_DATA)
        {
            painter->drawLine(startPos,endPos);
        }


    }
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   draw_label
* Description   :   画label
* Input         :   QPainter *painter：绘图设备
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::draw_label(QPainter *painter)
{
    Point p1, p2;

    if(isTouch)
    {
        p1 = touchPoint;
        if( currentPage != Page_24Hours )
            p2 = touchPointMinx;
    }
    else
    {
        int index = 0;
        if( currentPage == Page_24Hours )
        {
            while(listPoint12Hours[index].value == ERROR_DATA)
            {

                index++;
                if(index >= listPoint12Hours.size())
                {
                    return ;
                }

            }
            hoursValue->setText(QString("%1").arg(listPoint12Hours[index].value));
            p1 = listPoint12Hours[index];
        }
        else
        {
            while(listPoint7Days[0][index].value == ERROR_DATA || listPoint7Days[1][index].value == ERROR_DATA)
            {
                index++;
                if(index >= listPoint7Days[0].size())
                {
                    return ;
                }
            }
            daysMaxValue->setText(QString("%1").arg(listPoint7Days[0][index].value));
            daysMinValue->setText(QString("%1").arg(listPoint7Days[1][index].value));
            p1 = listPoint7Days[0][index];
            p2 = listPoint7Days[1][index];
        }
    }

    QPen pen = QPen(Qt::white,2, Qt::SolidLine, Qt::SquareCap);
    pen.setColor("#23a6de");
    painter->setPen(pen);
    if( currentPage == Page_24Hours )
        painter->drawLine(p1.posX ,p1.posY + 5,p1.posX,318);
    else
    {
        painter->drawLine(p1.posX ,p1.posY + 5,p1.posX,p2.posY);
        pen.setColor("#29d7c1");
        painter->setPen(pen);
        painter->drawLine(p1.posX ,p2.posY,p1.posX ,318);
    }

    painter->setBrush(QColor("#292a2c"));
    pen.setColor("#23a6de");
    pen.setWidth(7);
    painter->setPen(pen);
    painter->drawEllipse(p1.posX - 8,p1.posY -8 ,15, 15);

    if( currentPage != Page_24Hours )
    {
        pen.setColor("#29d7c1");
        painter->setPen(pen);
        painter->drawEllipse(p2.posX - 8,p2.posY -8 ,15, 15);
    }


}

void HistoryChartBase::change_index(int index)
{
    this->iCurrentIndex = index;
    waitWeatherData->stopAnimation();
    switch (index) {
    case PM_PAGE:
        iCurDataType = PM25;
        break;
    case TEMP_PAGE:
        iCurDataType = TEMP;
        break;
    case HUM_PAGE:
        iCurDataType = HUMI;
        break;
    case CO2_PAGE:
        iCurDataType = CO2E;
        break;
    case TVOC_PAGE:
        iCurDataType = TVOC;
        break;
    default:

        break;
    }
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
void HistoryChartBase::set_paintInit(bool flag)
{
    bPaintInit = flag;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_xRectList
* Description   :   设置7天位置信息
* Input         :   QList<QRect> rectList  位置列表
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::set_xRectList(QList<QRect> rectList)
{
    xRectList = rectList;

}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   isEmpty
* Description   :   判断七天或12小时数据是否为空
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool HistoryChartBase::isEmpty()
{
    //12小时

    if(currentPage == Page_24Hours)
    {
        for(int i = 0; i < 12; i++)
        {
            if(dataHours.nearestValue[i] != ERROR_DATA)
            {
                return false;
            }
        }

    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            if(dataDays.maxValue[i] != ERROR_DATA || dataDays.minValue[i] != ERROR_DATA)
            {
                return false;
            }
        }
    }
    return true;
}

void HistoryChartBase::set_labels( QLabel *hourValue,QLabel *maxValue, QLabel *minValue)
{
    hoursValue = hourValue;

    daysMaxValue = maxValue;
    daysMinValue = minValue;

}



void HistoryChartBase::get_daysData()
{
    waitWeatherData->startAnimation();
    messageBox->hide();
    bPaintInit = false;


    emit signal_get_historyDaily(iCurDataType);

}

void HistoryChartBase::get_hoursData()
{
    waitWeatherData->startAnimation();
    messageBox->hide();
    bPaintInit = false;
    emit signal_get_historyHourly(iCurDataType);

}


void HistoryChartBase::slot_update_historyDaily(Data7Days data)
{
    if(iCurDataType == data.type && currentPage == Page_7Days)
    {
        dataDays = data;
        bPaintInit = true;
        messageBox->hide();
        update();
        waitWeatherData->stopAnimation();

    }
}

void HistoryChartBase::slot_update_historyHourly(Data12Hours data)
{
    if(iCurDataType == data.type && currentPage == Page_24Hours)
    {
        dataHours = data;
        bPaintInit = true;
        messageBox->hide();
        update();
        waitWeatherData->stopAnimation();

    }
}



/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   mousePressEvent
* Description   :   鼠标按下事件
* Input         :   QMouseEvent *event 鼠标事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::mousePressEvent(QMouseEvent *event)
{    
    //点击事件处理
    touchEvent(event);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   mouseMoveEvent
* Description   :   鼠标移动事件
* Input         :   QMouseEvent *event 鼠标事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标移动事件处理
    touchEvent(event);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   mouseReleaseEvent
* Description   :   鼠标释放事件
* Input         :   QMouseEvent *event 鼠标事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标释放事件处理
    touchEvent(event);
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   touchEvent
* Description   :   鼠标按下或者移动时候事件
* Input         :   QMouseEvent *event 鼠标事件
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::touchEvent(QMouseEvent *event)
{

    //只处理鼠标按下和鼠标移动事件
    if(event ->type() != QEvent::MouseButtonRelease)
    {
        //修改鼠标按下或者移动标志位
        isTouch = true;

        //获取点击坐标
        touchPoint.posX = event->x();
        touchPoint.posY = event->y();

        //获取点击点附近的环境参数值
        get_nearestValue();
    }
    else
    {
        //修改鼠标按下或者移动标志位
        isTouch = false;
        touchPoint.posX = -100;
    }

    //刷新界面
    update();

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_nearestValue
* Description   :   获取鼠标点击位置附近的值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::get_nearestValue()
{
    //目标点和数模点的距离
    int iIndex = -1;
    if( currentPage == Page_24Hours)
    {

        if (listPoint12Hours.length() > 0)
        {

            iIndex = get_nearPointIndex(listPoint12Hours);
            //更新点击位置的坐标 环境值 和时间
            if(iIndex != -1)
            {
                touchPoint.posX = listPoint12Hours[iIndex].posX;
                touchPoint.posY = listPoint12Hours[iIndex].posY;
                touchPoint.value = listPoint12Hours[iIndex].value;
                touchPoint.time = listPoint12Hours[iIndex].time;

                hoursValue->setText(QString("%1").arg(touchPoint.value));
            }
        }

    }
    else
    {
        if(listPoint7Days[0].length() > 0 && listPoint7Days[1].length() > 0)
        {
            iIndex = get_nearPointIndex(listPoint7Days[0]);
            //更新点击位置的坐标 环境值 和时间
            if(iIndex != -1)
            {
                touchPoint.posX = listPoint7Days[0][iIndex].posX;
                touchPoint.posY = listPoint7Days[0][iIndex].posY;
                touchPoint.value = listPoint7Days[0][iIndex].value;
                touchPoint.time = listPoint7Days[0][iIndex].time;

                touchPointMinx.posX = listPoint7Days[1][iIndex].posX;
                touchPointMinx.posY = listPoint7Days[1][iIndex].posY;
                touchPointMinx.value = listPoint7Days[1][iIndex].value;
                touchPointMinx.time = listPoint7Days[1][iIndex].time;

                daysMaxValue->setText(QString("%1").arg(touchPoint.value));
                daysMinValue->setText(QString("%1").arg(touchPointMinx.value));
            }
        }
    }

}

int HistoryChartBase::get_nearPointIndex(QList<Point> &list)
{
    //目标点和数模点的距离
    int iSpace = 0;
    int iIndex = -1;
    for(int i = list.length() -1; i >=0; --i)
    {
        //循环坐标
        Point pos = list.at(i);
        float value = pos.value;
        //如果循环值为第一个值 或者 循环x坐标和点击x坐标差的绝对值小于记录差值时，记录差的绝对值并标记循环坐标下标
        if(value != ERROR_DATA && (iIndex  == -1 || qAbs(pos.posX - touchPoint.posX) < iSpace))
        {
            //记录循环x坐标和点击x坐标之间的差的绝对值
            iSpace = qAbs(pos.posX - touchPoint.posX);

            //标记该值
            iIndex = i;
        }
    }
    return iIndex;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   init_pen
* Description   :   初始化画笔
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void HistoryChartBase::init_pen()
{
    CG = new ColorGenerator();
    CG->setIndex(iCurrentIndex);
    penBaseText = QPen(QBrush(QColor("#ffffff")), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    penBaseLineH = QPen(Qt::white, 2);
    penBaseLineV = QPen(Qt::white, 2);
    QColor black = QColor(Qt::black);
    black.setAlpha(0.1 * 0xFF);
}

