#include "gamma.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    argument["type"] = "slider";
    argument["name"] = "gamma";
    argument["min"] = "0";
    argument["max"] = "5";
    argument["step"] = "0.01";
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Gamma";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Gamma();
}

Gamma::Gamma()
{

}

int Gamma::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    if(argv.size() < 1)
        return 2;
    qDebug()<<"ContrastV = "+argv.at(0).toInt();
    gamma(bitmap, n, argv.at(0).toDouble());
    return 0;
}

void Gamma::gamma(uchar* bitmap, size_t n, float value)
{
    uchar* RampTable = new uchar[256];
    //Вычислям общий коэффицент гаммы,который потребуется для вычисления главного значения
    double temp = 5.0 <= value ? 5.0 : value;
    double Gam = 0.1 >= temp ? 0.1 : temp;
    double G = 1 / Gam; //Главное значение гаммы

    for (int i = 0; i < 256; i++)
    {
        temp = (pow(i / 255.0, G) * 255 + 0.5);
        RampTable[i] = (uchar)(255 <= (int)temp ? 255 : (int)temp);
    }
    for (size_t i = 1; i < n-1; i += 4)
    {
        bitmap[i - 1] = RampTable[bitmap[i - 1]];
        bitmap[i] = RampTable[bitmap[i]];
        bitmap[i + 1] = RampTable[bitmap[i + 1]];
    }
    delete[] RampTable;
}
