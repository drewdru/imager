#include "GrayWorld.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "GrayWorld";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new GrayWorld();
}

GrayWorld::GrayWorld()
{
}

int GrayWorld::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    QSize imgSize;
    imgSize.setHeight(argv.at(1).toInt());
    imgSize.setWidth(argv.at(2).toInt());
    grayWorld(bitmap, n, imgSize);
    return 0;
}

void GrayWorld::grayWorld(uchar* bitmap, size_t n, QSize imgSize)
{
    double avgR = 0, avgG = 0, avgB = 0, avg = 0;
    size_t N = imgSize.width()*imgSize.height();
    for (size_t i = 1; i < n-1; i += 4)
    {
        avgR += bitmap[i - 1];
        avgG += bitmap[i];
        avgB += bitmap[i + 1];
    }
    avgR = (avgR)/N;
    avgG = (avgG)/N;
    avgB = (avgB)/N;
    if (avgR == 0) avgR = 0.1;
    if (avgG == 0) avgG = 0.1;
    if (avgB == 0) avgB = 0.1;
    avg = (avgR + avgG + avgB) / 3.0; // avg[0]

    for (size_t i = 1; i < n-1; i ++)
    {
        if((int)bitmap[i - 1] * (avg / avgB)>255)
            bitmap[i - 1] = 255;
        else
            bitmap[i - 1] = (int)bitmap[i - 1] * (avg / avgB);
        if((int)bitmap[i] * (avg / avgG)>255)
            bitmap[i] = 255;
        else
            bitmap[i] = (int)bitmap[i] * (avg / avgG);
        if((int)bitmap[i + 1] * (avg / avgR)>255)
            bitmap[i + 1] = 255;
        else
            bitmap[i + 1] = (int)bitmap[i + 1] * (avg / avgR);
    }

}

