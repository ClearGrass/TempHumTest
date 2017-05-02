#ifndef HISTORYCHARTDEFINE_H
#define HISTORYCHARTDEFINE_H

#define TOTAL_HOURS     12
#define TOTAL_VALUES    4

#define WEATHER_DAYS_PAGE_WIDTH    1050
#define WEATHER_HOURS_PAGE_WIDTH   1800
enum Page_Type
{
  Page_7Days = true,
  Page_24Hours = false
};

typedef struct Range
{
    float maxRange;
    float minRange;
}Range;

typedef struct Point
{
    float posX;
    float posY;
    QString time;
    float value;
}Point;

#endif // HISTORYCHARTDEFINE_H
