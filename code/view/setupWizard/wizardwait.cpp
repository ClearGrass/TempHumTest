#include "wizardwait.h"
#include "ui_wizardwait.h"

WizardWait::WizardWait(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WizardWait)
{
    ui->setupUi(this);

    //去掉title栏，和关闭按钮
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);


}

WizardWait::~WizardWait()
{
    delete ui;
}

