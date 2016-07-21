#include "gray.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Gray";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Gray();
}

Gray::Gray()
{
}

int Gray::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
//    if(argv.size() < 1)
//        return 2;
    gray(bitmap, n);
    return 0;
}

void Gray::gray(uchar* bitmap, size_t n)
{
    for (size_t i = 1; i < n-1; i += 4)
    {
        double max = bitmap[i - 1] >= bitmap[i] ? bitmap[i - 1] : bitmap[i];
        max = max >= bitmap[i + 1] ? max : bitmap[i + 1];
        double temp = ((int)bitmap[i - 1] + (int)bitmap[i] + (int)bitmap[i+1]);
        if(temp==0) temp = 0.1;
        bitmap[i - 1] = max;
        bitmap[i] = max;
        bitmap[i + 1] = max;
    }
}

