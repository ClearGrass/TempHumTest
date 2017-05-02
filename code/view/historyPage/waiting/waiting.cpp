#include "waiting.h"
#include "ui_waiting.h"
Waiting::Waiting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Waiting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    label1= new LabelRotate(this,":/resource/images/optionPage/tools/loading.png",true);
    this->hide();
}

Waiting::~Waiting()
{
    delete ui;
}

void Waiting::startAnimation()
{
    this->show();
    label1->startAnimation(10);

}

void Waiting::stopAnimation()
{
    label1->stopAnimation();
    this->hide();
}
