#ifndef HISTORYCHARTBASE_H
#define HISTORYCHARTBASE_H
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDateTime>
#include <QMouseEvent>
#include <QTimer>
#include "globalVariable.h"
#include "colorGenerator.h"
#include "historyChartDefine.h"
#include "math.h"
#include "sysControl.h"
#include "waiting.h"
#include "dataAirControl.h"
#include "control/function/weather/funcWeather.h"

#define TOP_Y  80                                                           //
#define BOTTOM_Y 280                                                        //

class HistoryChartBase : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryChartBase(int index,QWidget *parent = 0,Page_Type currentPage = Page_7Days);
    void init_points();                                                     //初始化点
    void init_data();
    void init_pos();                                                        //初始化坐标信息
    void init_range();                                                      //初始化坐标轴值域
    void init_range(QList<float> &listValue);
    void init_pen();                                                        //初始化画笔
    void paintEvent(QPaintEvent *);                                         //重绘界面事件
    void draw_points(QPainter* painter);                                    //绘制点
    void draw_points(QPainter *painter,QList<Point>& pointList);
    void draw_line_points(QPainter *painter,QList<Point>& pointList);
    void draw_range(QPainter* painter);                                     //绘制坐标轴
    void draw_times(QPainter* painter);                                     //绘制时间轴
    void draw_label(QPainter* painter);                                     //绘制label
    void touchEvent(QMouseEvent *event);                                    //点击事件
    void mousePressEvent(QMouseEvent *event);                               //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);                                //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);                             //鼠标释放事件
    void get_nearestValue();                                                //获取点击附近的值
    int get_nearPointIndex(QList<Point> &list);
    void change_index(int index);                                           //改变环境 index
    void set_paintInit(bool);                                               //设置页面数据重绘
    void set_xRectList(QList<QRect> rectList);                              //设置7天位置信息、
    bool isEmpty();

    void set_labels(QLabel *hourValue , QLabel *maxValue, QLabel *minValue);

    QFrame *hoursFrame;
    QLabel *hoursValue;

    QLabel *daysMaxValue;
    QLabel *daysMinValue;
private:
    void get_daysData();
    void get_hoursData();


private slots:
    void slot_update_historyDaily(Data7Days);                              //刷新7天数据
    void slot_update_historyHourly(Data12Hours);                           //刷新12小时数据
private:

signals:
    void signal_get_historyDaily(AirDataType);
    void signal_get_historyHourly(AirDataType);
    void signal_get_weather();
    void signal_unavailable(bool);
private:
    ColorGenerator *CG;                                                     //获取颜色对象
    QDateTime currentTime;                                                  //当前时间
    int iDay;
    int iHour;
    int iMinute;
    float iMaxRange;
    float iMinRange;
    bool bPaintInit;                                                        //是否初始化位图
    QList<Point> listPoint12Hours;                                          //24小时数据
    QList<Point> listPoint7Days[2];                                         //7天数据
    DataAirControl *dataAirControl;

    QRectF rectText;
    QPen penPointL;
    QPen penPointS;
    QPen penBaseText;
    QPen penBaseLineH;
    QPen penBaseLineV;
    Point touchPoint;                                                       //点击位置坐标
    Point touchPointMinx;
    int touchDay;                                                           //点击位置 day
    int iCurrentIndex;                                                      //当前页面环境index保存
    float range[TOTAL_VALUES];
    static Range rangeDefalut[5];                                           //默认的坐标轴范围
    Page_Type currentPage;                                                  //当前页面

    QList<QRect> xRectList;                                                 //7天初始位置信息列表

    Data12Hours  dataHours;
    Data7Days    dataDays;
    AirDataType  iCurDataType;
    SysControl  *sysControl;
    bool isTouch;                                                           //是否被点击
    Waiting *waitWeatherData;
    QLabel *messageBox;
    QFrame *dayFrame;                                                       //七天容器
    bool   isUsLang;                                                        //是否为英文
};

#endif // HISTORYCHARTBASE_H
