#ifndef WEATHERCHARTBASE_H
#define WEATHERCHARTBASE_H
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

#define X0                         75
#define X_SPACE                    150
#define FONT_FAMILY                "MI LANTING"
#define DAYS_Y_TOP                  30
#define DAYS_VALUE_Y_TOP            71
#define ICON_SIZE                   31
#define DAYS_TEXT_WIDRH             60
#define DAYS_TEXT_HEIGHT            40
#define DAYS_VALUE_Y_BOTTOM         258

#define DAYS_LINE_Y_TOP             121
#define DAYS_LINE_Y_BOTTOM          248


#define HOURS_Y_TOP                 90
#define HOURS_Y_BOTTOM              217
class WeatherChartBase : public QWidget
{
    Q_OBJECT
public:
    explicit WeatherChartBase(QWidget *parent = 0,Page_Type currentPage = Page_7Days);
    void init_points();                                                     //初始化点
    void init_range();                                                      //初始化坐标轴值域
    void init_range(QList<float> &listValue);
    void paintEvent(QPaintEvent *);                                         //重绘界面事件
    void draw_points(QPainter* painter);                                    //绘制点
    void draw_points(QPainter *painter,QList<Point>& pointList);
    void draw_range(QPainter* painter);                                     //绘制坐标轴
    void draw_maxAndMin(QPainter *painter);                                 //绘制7天最大值最小值
    void draw_times(QPainter* painter);                                     //绘制时间轴
    void draw_weatherIcon(QPainter* painter);
    void set_paintInit(bool);                                               //设置页面数据重绘
    void set_7daysRect(QList<QRect> rectList);                              //设置7天位置信息
    void refresh_data();
    QString get_weatherDescribe(QString des);
private slots:
    void slot_update_weatherDaily(QList<WeatherDaily> *weatherDaily);       //天气预报七天数据
    void slot_update_weatherHourly(QList<WeatherHourly> *weatherHours);     // 12小时数据
    void slot_net_abnormal();
    void slot_net_disconnect();                                             // 网络断开
private:

signals:
    void signal_get_weather();

private:
    ColorGenerator *CG;                                                     //获取颜色对象
    QDateTime currentTime;                                                  //当前时间
    float iMaxRange;
    float iMinRange;
    bool bPaintInit;                                                        //是否初始化位图
    QList<Point> listPoint12Hours;                                          //24小时数据
    QList<Point> listPoint7Days[2];                                         //7天数据
    FuncWeather *funcWeather;

    static Range rangeDefalut[5];                                           //默认的坐标轴范围
    Page_Type currentPage;                                                  //当前页面

    QList<QRect> m_7DaysRectList;                                            //7天初始位置信息列表
    SysControl  *sysControl;
    QList<WeatherDaily> listWeatherDaily;
    QList<WeatherHourly> listWeatherHours;
    Waiting *waitWeatherData;
    QLabel *messageBox;
    bool   isUsLang;                                                        //是否为英文



};

#endif // WEATHERCHARTBASE_H
