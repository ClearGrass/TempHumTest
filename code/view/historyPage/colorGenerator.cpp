#include "colorGenerator.h"
#include <QColor>
ColorGenerator::ColorGenerator()
{
    iCurrentIndex =PM_PAGE;
    if(iCurrentIndex == PM_PAGE)
    {
        this->set_startColor(0x4dd94c);
        this->set_color(0.3333, 0xffab00);
        this->set_color(0.5, 0xff5700);
        this->set_color(0.7, 0xc80900);
        this->set_endColor(0xb80d57);
        this->minValue = 0;
        this->maxValue = 200;
    }
}
void ColorGenerator::set_color(float point, int hexRGB)
{
    RGB rgb;
    if (pointColorMap.contains(point)) {
        rgb = pointColorMap[point];
    } else {
        pointColorMap[point] = rgb;
    }
    if (!points.contains(point)) {
        points.append(point);
    }
    int red, green, blue;
    parse_hexToRGB(hexRGB, red, green, blue);
    rgb.setColor(red, green, blue);
    pointColorMap[point] = rgb;
    qSort(points.begin(), points.end());
}



void ColorGenerator::set_startColor(int hexRGB)
{
    set_color(0, hexRGB);
}
void ColorGenerator::set_endColor(int hexRGB)
{
    set_color(1, hexRGB);
}

void ColorGenerator::parse_hexToRGB(int hex, int& red, int& green, int& blue)
{
    QString hexString = "00000" + QString::number(hex, 16);
    int length = hexString.length();

    red = hexString.mid(length - 6, 2).toInt(0, 16);
    green = hexString.mid(length - 4, 2).toInt(0, 16);
    blue = hexString.mid(length - 2, 2).toInt(0, 16);
    //    qDebug() << "parse" << hex << hexString << "red" << red << "green" << green << "blue" << blue;
}

QPair<float, float> ColorGenerator::find_rangRgb(float ratio, QMap<float, RGB> pointColorMap) {
    QPair<float, float> res;
    res.first = 0;
    res.second = 1;
    //    return res;
    if (ratio <= 0) {
        res.first = 0;
        res.second = 1;
        return res;
    }
    if (ratio >= 1)  {
        res.first = 0;
        res.second = 1;
        return res;
    }
    QList<float> points = pointColorMap.keys();
    qSort(points.begin(), points.end());
    for (int i = 0; i<points.size() - 1; i ++) {
        if (ratio > points[i] && ratio <= points[i+1]) {
            res.first = points[i];
            res.second = points[i + 1];
            return res;
        }
    }
    return res;
}

QString ColorGenerator::get_color(float ratio)
{
    QPair<float, float> colors = find_rangRgb(ratio, pointColorMap);

    //    qDebug() << "ratio" << ratio << colors.first << colors.second;
    RGB startColor = pointColorMap[colors.first];
    RGB endColor = pointColorMap[colors.second];

    float realRatio = (ratio - colors.first) / (colors.second - colors.first);
    int red = (int)((realRatio - 0)*endColor.red + (1-realRatio)*startColor.red);
    int green = (int)((realRatio - 0) *endColor.green + (1-realRatio)*startColor.green);
    int blue = (int)((realRatio - 0)*endColor.blue + (1-realRatio)*startColor.blue);
    int color = (red << 16) | (green <<8) | blue;
    QString colorString = QString::number(color, 16);
    colorString = (colorString.length() >= 6 ? ("#" + colorString.right(6)) : ("#0" + colorString));
    //    qDebug() << "ratio" << ratio << realRatio << "color" << color << colorString << "red" << red << "green" << green << "blue" << blue;
    return colorString;
}


QColor ColorGenerator::color(float value)
{
    float ratio = (value < minValue) ? 0 : (value > maxValue ?
                                                1 : ((value - minValue) / (maxValue - minValue)));
    QString colorStr = get_color(ratio);
    return QColor(colorStr);
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-05
* Function Name :   setIndex
* Description   :   根据页面设置渐变
* Input         :   int index当前页面
* Output        :   None
* Return        :   获取提示信息
*******************************************************************************/
void ColorGenerator::setIndex(int index)
{
    //清空
    points.erase(points.begin(), points.end());
    pointColorMap.clear();
    iCurrentIndex =index;
    switch (index) {
    //pm2.5
    case PM_PAGE :
        this->set_startColor(0x4dd94c);
        this->set_color(0.3333, 0xffab00);
        this->set_color(0.5, 0xff5700);
        this->set_color(0.7, 0xc80900);
        this->set_endColor(0xb80d57);
        this->minValue = 0;
        this->maxValue = 250;
        break;
        //温度
    case TEMP_PAGE :
    case WEATHRT_PAGE :
        this->set_startColor(0x4dd94c);
        this->set_endColor(0xffab00);
        this->minValue = 0;
        this->maxValue = 30;
        break;
        //湿度
    case HUM_PAGE :
        this->set_startColor(0x4dd94c);
        this->set_endColor(0xffab00);
        this->minValue = 0;
        this->maxValue = 30;
        break;
        //co2e
    case CO2_PAGE :
        this->set_startColor(0x4dd94c);
        this->set_color(0.3333, 0xffab00);
        this->set_color(0.6666, 0xff5700);
        this->set_endColor(0xc80900);
        this->minValue = 0;
        this->maxValue = 5000;
        break;
        //tVOC
    case TVOC_PAGE :
        this->set_startColor(0x4dd94c);
        this->set_color(0.5, 0xff5700);
        this->set_endColor(0xc80900);
        this->minValue = 0;
        this->maxValue = 5;
        break;

    default:
        break;
    }
}

