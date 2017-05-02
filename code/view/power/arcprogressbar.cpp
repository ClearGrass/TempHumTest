#include "arcprogressbar.h"

ArcProgressBar::ArcProgressBar(QWidget *parent) : QWidget(parent)
{
    updateTimer = new QTimer(this);
    updateTimer->setInterval(14);
    resize(170, 170);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    currentValue = 0;
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-
* Function Name :   start_timer
* Description   :   开启定时器
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void ArcProgressBar::start_timer()
{
    updateTimer->start();

}

/*******************************************************************************
* Author        :   虎正玺@2017-02-
* Function Name :   stop_timer
* Description   :   关闭定时器
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void ArcProgressBar::stop_timer()
{
    updateTimer->stop();
    currentValue = 0;
    update();
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-
* Function Name :   slotUpdateTimer
* Description   :   定时器槽函数
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void ArcProgressBar::slotUpdateTimer()
{
    //扫描一圈后，停止定时器，修改扫描角度为0
    if (currentValue >= 360) {
        updateTimer->stop();
        currentValue = 0;
        update();
        return;
    }

    //修改扫描角度
    currentValue++;

    //重新绘制
    update();
}
void ArcProgressBar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int radius = 85;
    int arcHeight = 10;

    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, height() >> 1);

    gradientArc(&painter, radius, 0, -currentValue, arcHeight, qRgb(0, 229, 255));

}

/*******************************************************************************
* Author        :   虎正玺@2017-02-22
* Function Name :   gradientArc
* Description   :   radius 半径 startAngle ，开始角度 angleLength 指扫取的角度 ，arcHeight 圆环的高度， color：填充色
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void ArcProgressBar::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{

    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);

    // << 1（左移1位）相当于radius*2 即：150*2=300
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);

    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}
