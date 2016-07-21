#include "Binarization.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{    
    QJsonObject argument;
    argument["type"] = "slider";
    argument["name"] = "limit";
    argument["min"] = "0";
    argument["max"] = "765";
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Binarization";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Binarization();
}

Binarization::Binarization()
{

}

int Binarization::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    if(argv.size() < 1)
        return 2;
    Binarize(bitmap, n, argv.at(0).toInt());
    return 0;
}

void Binarization::Binarize(uchar* bitmap, size_t n, int limit)
{
    for (size_t i = 1; i < n; i += 4)
    {
        int res = (int)bitmap[i-1]+(int)bitmap[i]+(int)bitmap[i+1];
        if(res<=limit)
        {
            bitmap[i-1] = 0;
            bitmap[i] = 0;
            bitmap[i+1] = 0;
        }
        else
        {
            bitmap[i-1] = 255;
            bitmap[i] = 255;
            bitmap[i+1] = 255;
        }
    }
}
