#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QButtonGroup>
#include "database.h"
#include "globalVariable.h"
#include "historyChartBase.h"
#include <QLabel>
#include <QDialog>
#include "SwitchControl.h"
#include "weatherChartBase.h"
#include "control/function/city/funcCity.h"

namespace Ui
{
    class HistoryPage;
}

class HistoryPage : public QDialog
{
    Q_OBJECT

public:

    explicit HistoryPage(QWidget *parent = 0);
    ~HistoryPage(void);
    void set_index(int index);                                          //设置当前页面

private slots:

    void on_button_back_clicked();                                      //返回按钮
    void slot_switchControlClicked(bool);
    void slot_unavailable(bool);
signals:
    void signal_backDefaultPage(void);                                  // back信号


private:
    QRect getAbsoluteRect(QRect rect);
    void init_coordinateAxis();
    void set_unit(int index);
    void set_labelVisible(bool isVisible);

    void mousePressEvent(QMouseEvent *event);                               //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);                                //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);                             //鼠标释放事件
    void move_subWigget(int space);
private:

    HistoryChartBase *historyChart24Hours;                              //24小时页面实例
    HistoryChartBase *historyChart7Days;                                //7天页面实例

    WeatherChartBase *weatherHours;
    WeatherChartBase *weatherDays;
    Ui::HistoryPage *ui;                                                //历史页面ui实例
    FuncCity    *funcCity;

    int currentPage;
    SwitchControl *switchControl;
    QList<QLabel *>  listLabel;                                         //7天列表
    SysControl  *sysControl;
    bool isHours;
    QPoint prevPoint;

private:




};


#endif // HISTORYPAGE_H
