#ifndef LABELROTATE_H
#define LABELROTATE_H
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPainter>
#include <QMatrix>
class LabelRotate : public QLabel
{
    Q_OBJECT
public:
    explicit LabelRotate(QWidget *parent = 0,QString pix="",bool d=true);
    void setMinLoops(int loops = 3);

    void startAnimation(int interval = 25);
    void stopAnimation();

signals:
    void runMinLoopsEnough();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *event);

private:
    int timerID;
    int minLoops;
    QPixmap m_pixmap;
    qreal m_rotation;
    bool direction;
};

#endif // LABELROTATE_H
