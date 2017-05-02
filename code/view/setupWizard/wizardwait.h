#ifndef WIZARDWAIT_H
#define WIZARDWAIT_H

#include <QWidget>
namespace Ui {
class WizardWait;
}

class WizardWait : public QWidget
{
    Q_OBJECT

public:
    explicit WizardWait(QWidget *parent = 0);
    ~WizardWait();

private:
    Ui::WizardWait *ui;
};

#endif // WIZARDWAIT_H
