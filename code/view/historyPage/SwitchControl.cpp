#include <QPainter>
#include <QMouseEvent>
#include "SwitchControl.h"

SwitchControl::SwitchControl(QWidget *parent)
    : QWidget(parent),
      m_nHeight(56),
      m_bChecked(true),
      m_radius(28.0),
      m_thumbColor("#3c3d40"),
      m_background("#6d6e70")
{
    // 鼠标滑过光标形状 - 手型
    setCursor(Qt::PointingHandCursor);
    font.setFamily("MI LANTING");
    font.setPixelSize(24);
    daysText = tr("7 Days");
    hoursText = tr("12 Hours");
    m_nY = 0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(slot_onTimeout()));

}

// 绘制开关
void SwitchControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setFont(font);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QPainterPath path2;

    // 绘制大椭圆
    painter.setBrush(m_background);
    path.addRoundedRect(QRectF(0, 0, width(), height()), m_radius, m_radius);
    painter.drawPath(path.simplified());

    // 绘制小椭圆
    painter.setBrush(m_thumbColor);
    path.addRoundedRect(QRectF(m_nX, m_nY, width() / 2, height()), m_radius, m_radius);

    painter.drawPath(path.simplified());

    if(m_bChecked)
    {
        painter.setPen(QPen(QColor("#ffffff")));
        painter.drawText(QRect(0, 0,width() / 2, height()), Qt::AlignCenter,hoursText);
        painter.setPen(QPen(QColor("#A9AAAA")));
        painter.drawText(QRect(width() / 2, 0,width() / 2, height()), Qt::AlignCenter, daysText);
    }
    else
    {
        painter.setPen(QPen(QColor("#A9AAAA")));
        painter.drawText(QRect(0, 0,width() / 2, height()),  Qt::AlignCenter, hoursText);
        painter.setPen(QPen(QColor("#ffffff")));
        painter.drawText(QRect(width() / 2, 0,width() / 2, height()), Qt::AlignCenter, daysText);
    }


}

// 鼠标按下事件
void SwitchControl::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

// 鼠标释放事件 - 切换开关状态、发射toggled()信号
void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            m_bChecked = !m_bChecked;
            update();
            emit toggled(m_bChecked);
            timer.start(10);

        } else {
            event->ignore();
        }
    }
}



// 返回开关状态 - 12小时：true 7天：false
bool SwitchControl::isHoursChecked() const
{
    return m_bChecked;
}

// 设置开关状态
void SwitchControl::setToggle(bool checked)
{
    m_bChecked = checked;
    m_nX = checked ? 0 : width()/2;
    update();
}



void SwitchControl::slot_onTimeout()
{
    if (!m_bChecked) {
        m_nX += 10;
        if (m_nX > width()/2)
        {
            m_nX =  width()/2;
            timer.stop();
        }
    } else {
        m_nX -= 10;
        if (m_nX <  0)
        {
            m_nX = 0;
            timer.stop();
        }
    }
    update();
}

