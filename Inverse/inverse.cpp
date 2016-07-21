#include "inverse.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Inverse";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Inverse();
}

Inverse::Inverse()
{
}

int Inverse::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    inverse(bitmap, n);
    return 0;
}

void Inverse::inverse(uchar* bitmap, size_t n)
{
    RGBToYUV(bitmap, n);
    for (size_t i = 0; i < n; i += 4)
    {
        bitmap[i] = 255 - bitmap[i];
    }
    YUVToRGB(bitmap, n);
}


void Inverse::RGBToYUV(uchar* bitmap, size_t n)
{
    //Y = 0.299 * R + 0.587 * G + 0.114 * B;
    //U = -0.14713 * R - 0.28886 * G + 0.436 * B + 128;
    //V = 0.615 * R - 0.51499 * G - 0.10001 * B + 128;
    for (size_t i = 1; i < n-1; i+=4)
    {
        double Y = 0.299 * bitmap[i - 1] + 0.587 * bitmap[i] + 0.114 * bitmap[i+1];
        double U = -0.14713 * bitmap[i - 1] - 0.28886 * bitmap[i] + 0.436 * bitmap[i + 1] + 128;
        double V = 0.615 * bitmap[i - 1] - 0.51499 * bitmap[i] - 0.10001 * bitmap[i + 1] + 128;

        if (Y>=0&&Y<=255) bitmap[i - 1] = Y;
        else if (Y<0) bitmap[i - 1] = 0;
        else bitmap[i - 1] = 255;

        if (U>=0&&U<=255) bitmap[i] = U;
        else if (U<0) bitmap[i] = 0;
        else bitmap[i] = 255;

        if (V>=0&&V<=255) bitmap[i + 1] = V;
        else if (V<0) bitmap[i + 1] = 0;
        else bitmap[i + 1] = 255;
    }
}

void Inverse::YUVToRGB(uchar* bitmap, size_t n)
{
    //R = Y + 1.13983 * (V - 128);
    //G = Y - 0.39465 * (U - 128) - 0.58060 * (V - 128);
    //B = Y + 2.03211 * (U - 128);
    for (size_t i = 1; i < n - 1; i += 4)
    {
        double R = bitmap[i - 1] + 1.13983 * (bitmap[i + 1] - 128);
        double G = bitmap[i - 1] - 0.39465 * (bitmap[i] - 128) - 0.58060 * (bitmap[i + 1] - 128);
        double B = bitmap[i - 1] + 2.03211 * (bitmap[i] - 128);

        if (R>=0&&R<=255) bitmap[i - 1] = R;
        else if (R<0) bitmap[i - 1] = 0;
        else bitmap[i - 1] = 255;

        if (G>=0&&G<=255) bitmap[i] = G;
        else if (G<0) bitmap[i] = 0;
        else bitmap[i] = 255;

        if (B>=0&&B<=255) bitmap[i + 1] = B;
        else if (B<0) bitmap[i + 1] = 0;
        else bitmap[i + 1] = 255;
    }
}

