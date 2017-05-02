#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H
#include <QMap>
#include <QString>
#include <QPair>
#include <QtDebug>
#include "globalVariable.h"
struct RGB {
    RGB() {}
    RGB(int red, int green, int blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    void setColor(int red, int green, int blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    int red;
    int green;
    int blue;
};


class ColorGenerator
{

public:
    explicit ColorGenerator();

    void set_color(float point , int hexRGB);
    void set_startColor(int hexRGB);
    void set_endColor(int hexRGB);
    void set_startEndColor(int hex1, int hex2) {
        set_startColor(hex1);
        set_endColor(hex2);
    }
    QString get_color(float point);
    QColor color(float value);

    void setIndex(int  index);

private:
    void parse_hexToRGB(int hex, int& red, int& green, int& blue);
    QPair<float, float> find_rangRgb(float ratio, QMap<float, RGB> pointColorMap);
    QList<float> points;
    QMap<float, RGB> pointColorMap;

    int minValue;
    int maxValue;
    int iCurrentIndex;

};

#endif // COLORGENERATOR_H
