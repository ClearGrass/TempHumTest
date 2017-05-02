#include "labelRotate.h"
#include <QDebug>
#include <QTimerEvent>
LabelRotate::LabelRotate(QWidget *parent,QString pix,bool d)
    :QLabel(parent)
{
    this->setGeometry(0,0,180,180);
    m_rotation = 0.0;
    timerID = 0;
    m_pixmap = QPixmap(pix);
    this->setAlignment(Qt::AlignCenter);
    this->setStyleSheet("background:transparent;");
    direction =d;
}
void LabelRotate::setMinLoops(int loops)
{
    minLoops = loops;
}

void LabelRotate::startAnimation(int interval)
{
    if(timerID == 0){
        timerID = startTimer(interval);
    }
}

void LabelRotate::stopAnimation()
{
    killTimer(timerID);
    timerID = 0;
}

void LabelRotate::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == timerID){
        if(direction)
        {
            m_rotation += 5;
        }
        else
        {
            m_rotation -= 5;
        }

        update();

        if(m_rotation > minLoops*360){
            emit runMinLoopsEnough();
        }
    }
}

void LabelRotate::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF center(m_pixmap.width() / qreal(2), m_pixmap.height() / qreal(2));
//    painter.drawPixmap(m_pixmap.width() / qreal(2) -leadon_text.width() / 2 , m_pixmap.height() / qreal(2), leadon_text);

    painter.translate(center);
    painter.rotate(m_rotation);
    painter.translate(-center);

    painter.drawPixmap(QPointF(0, 0), m_pixmap);
}
