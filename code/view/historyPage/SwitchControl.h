#ifndef SWITCH_CONTROL
#define SWITCH_CONTROL

#include <QWidget>
#include <QTimer>

class SwitchControl : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchControl(QWidget *parent = 0);

    // 返回开关状态 - 打开：true 关闭：false
    bool isHoursChecked() const;

    // 设置开关状态
    void setToggle(bool checked);


protected:
    // 绘制开关
    void paintEvent(QPaintEvent *event) ;

    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event) ;

    // 鼠标释放事件 - 切换开关状态、发射toggled()信号
    void mouseReleaseEvent(QMouseEvent *event) ;


signals:
    // 状态改变时，发射信号
    void toggled(bool checked);

private slots:
    // 状态切换时，用于产生滑动效果
    void slot_onTimeout();

private:
    bool m_bChecked;         // 是否选中
    QColor m_background;     // 背景颜色
    QColor m_thumbColor;     // 拇指颜色
    qreal m_radius;          // 圆角
    qreal m_nX;              // x点坐标
    qreal m_nY;              // y点坐标
    qint16 m_nHeight;        // 高度
    QFont font;
    QString daysText;
    QString hoursText;
    QTimer timer;          // 定时器

};

#endif // SWITCH_CONTROL
