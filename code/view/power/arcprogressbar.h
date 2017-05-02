#ifndef ARCPROGRESSBAR_H
#define ARCPROGRESSBAR_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QRadialGradient>
class ArcProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ArcProgressBar(QWidget *parent = 0);
    void start_timer();
    void stop_timer();
private slots:
    void slotUpdateTimer();
private:
    void paintEvent(QPaintEvent *e);
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);
private:
    QTimer *updateTimer;
    int currentValue;
};

#endif // ARCPROGRESSBAR_H
