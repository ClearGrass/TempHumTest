#ifndef WAITING_H
#define WAITING_H

#include <QWidget>
#include "labelRotate.h"

namespace Ui {
class Waiting;
}

class Waiting : public QWidget
{
    Q_OBJECT

public:
    explicit Waiting(QWidget *parent = 0);
    ~Waiting();
    void startAnimation();
    void stopAnimation();
private:
    Ui::Waiting *ui;
    LabelRotate *label1;

};

#endif // WAITING_H
