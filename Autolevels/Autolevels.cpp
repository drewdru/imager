#include "Autolevels.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QtAlgorithms>
extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
//    argument["type"] = "slider";
//    argument["name"] = "contrast";
//    argument["min"] = "0";
//    argument["max"] = "1019";
    QJsonArray argumentList;
    argumentList.append(argument);
    json["arguments"] = argumentList;
    json["pluginName"] = "Autolevels";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Autolevels();
}

Autolevels::Autolevels()
{
}

int Autolevels::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
//    if(argv.size() < 1)
//        return 2;
    autolevels(bitmap, n);
    return 0;
}

void Autolevels::autolevels(uchar* bitmap, size_t n)
{
    uchar minR = bitmap[0], minG = bitmap[1], minB = bitmap[2];
    uchar maxR = bitmap[0], maxG = bitmap[1], maxB = bitmap[2];
    for (size_t i = 5; i < n-1; i += 4)
    {
        minR = qMin(bitmap[i - 1],minR);
        minG = qMin(bitmap[i],minG);
        minB = qMin(bitmap[i],minB);

        maxR = qMax(bitmap[i - 1],maxR);
        maxG = qMax(bitmap[i],maxG);
        maxB = qMax(bitmap[i],maxB);
    }

    for (size_t i = 1; i < n-1; i += 4)
    {
        bitmap[i - 1] = (bitmap[i - 1] - minR)*(255/(maxR - minR));
        bitmap[i] = (bitmap[i] - minG)*(255/(maxG - minG));
        bitmap[i + 1] = (bitmap[i + 1] - minB)*(255/(maxB - minB));
        if(bitmap[i + 1] > bitmap[i - 1] && bitmap[i + 1] > bitmap[i]) bitmap[i+1] = bitmap[i];
    }
}
