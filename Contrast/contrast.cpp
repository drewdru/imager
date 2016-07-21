#include "contrast.h"


#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    argument["type"] = "slider";
    argument["name"] = "contrast";
    argument["min"] = "0";
    argument["max"] = "255";
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Contrast";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Contrast();
}

Contrast::Contrast()
{

}

int Contrast::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    if(argv.size() < 1)
        return 2;
    qDebug()<<"ContrastV = "+argv.at(0).toInt();
    contrast(bitmap, n, argv.at(0).toInt());
    return 0;
}

void Contrast::contrast(uchar* bitmap, size_t n, int threshold)
{
    double contrastLevel = pow((100.0 + threshold) / 100.0, 2);

    double blue = 0;
    double green = 0;
    double red = 0;

    for (size_t k = 1; k < n-1; k += 4)
    {
        blue = ((((bitmap[k-1] / 255.0) - 0.5) *
                 contrastLevel) + 0.5) * 255.0;


        green = ((((bitmap[k] / 255.0) - 0.5) *
                 contrastLevel) + 0.5) * 255.0;


        red = ((((bitmap[k + 1] / 255.0) - 0.5) *
               contrastLevel) + 0.5) * 255.0;

        if  (blue > 255)
        { blue = 255; }
        else if  (blue < 0)
        { blue = 0; }


        if (green > 255)
        { green = 255; }
        else if (green < 0)
        { green = 0; }


        if (red > 255)
        { red = 255; }
        else if (red < 0)
        { red = 0; }

        bitmap[k - 1] = (uchar)blue;
        bitmap[k] = (uchar)green;
        bitmap[k + 1] = (uchar)red;

    }

}
