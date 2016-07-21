#include "histogramequalization.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QtAlgorithms>
extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    QJsonArray argumentList;
    argumentList.append(argument);
    json["arguments"] = argumentList;
    json["pluginName"] = "HistogramEqualization";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new HistogramEqualization();
}

HistogramEqualization::HistogramEqualization()
{
}

int HistogramEqualization::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    QSize imgSize;
    imgSize.setHeight(argv.at(1).toInt());
    imgSize.setWidth(argv.at(2).toInt());

    histogramEqualization(bitmap, n, imgSize);
    return 0;
}

void HistogramEqualization::histogramEqualization(uchar* bitmap, size_t n, QSize imgSize)
{
    size_t N = imgSize.width()*imgSize.height();
    int k = 256;
    double* histogrammR = new double[k];
    double* histogrammG = new double[k];
    double* histogrammB = new double[k];
    for (int i = 0; i<k; i++)
    {
        histogrammR[i] = 0;
        histogrammG[i] = 0;
        histogrammB[i] = 0;
    }

    for (size_t i = 1; i < n-1; i+=4)
    {
        ++histogrammR[(int)bitmap[i - 1]];
        ++histogrammG[(int)bitmap[i]];
        ++histogrammB[(int)bitmap[i + 1]];
    }
    for (int i = 0; i<k; i++)
    {
        histogrammR[i] = histogrammR[i]/N * i;
        histogrammG[i] = histogrammG[i]/N * i;
        histogrammB[i] = histogrammB[i]/N * i;
//        qDebug()<<histogrammR[i];
    }
    for (int i = 1; i<k; i++)
    {
        histogrammR[i] = histogrammR[i-1] + histogrammR[i];
        histogrammG[i] = histogrammG[i-1] + histogrammG[i];
        histogrammB[i] = histogrammB[i-1] + histogrammB[i];

    }

    for (size_t i = 1; i < n-1; i ++)
    {
        int R = (uchar)round(histogrammR[(int)bitmap[i - 1]]);
        if ((bitmap[i-1] + R >= 0) && (bitmap[i-1] + R <= 255)) bitmap[i-1] += R;
        else if (bitmap[i-1] + R < 0) bitmap[i-1] = 0;
        else bitmap[i-1] = 255;

        int G = (uchar)round(histogrammG[(int)bitmap[i]]);
        if ((bitmap[i] + G >= 0) && (bitmap[i] + G <= 255)) bitmap[i] += G;
        else if (bitmap[i] + G < 0) bitmap[i] = 0;
        else bitmap[i] = 255;

        int B = (uchar)round(histogrammB[(int)bitmap[i + 1]]);
        if ((bitmap[i+1] + B >= 0) && (bitmap[i+1] + B <= 255)) bitmap[i+1] += B;
        else if (bitmap[i+1] + B < 0) bitmap[i+1] = 0;
        else bitmap[i+1] = 255;
    }
    delete[] histogrammR;
    delete[] histogrammG;
    delete[] histogrammB;
}
