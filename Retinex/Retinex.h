#ifndef RETINEX_H
#define RETINEX_H

#include "retinex_global.h"

#include <math.h>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QImage>

#include <QDebug>

#define PLUGIN_VERSION 1.0

class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};

class PM_WIDGET_EXPORT Retinex : public ProcessImageInterface
{
public:
    Retinex();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void retinex(uchar* bitmap, size_t n, QSize imgSize, int cbCurIndex);
    void RGBToYUV(uchar *bitmap, size_t n);
    void YUVToRGB(uchar* bitmap, size_t n);
};

#endif // RETINEX_H
