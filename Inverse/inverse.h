#ifndef INVERSE_H
#define INVERSE_H

#include "inverse_global.h"

#include <math.h>
#include <QVector>
#include <QString>

#define PLUGIN_VERSION 1.0

class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};

class PM_WIDGET_EXPORT Inverse : public ProcessImageInterface
{
public:
    Inverse();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void inverse(uchar* bitmap, size_t n);
    void RGBToYUV(uchar *bitmap, size_t n);
    void YUVToRGB(uchar* bitmap, size_t n);
};

#endif // INVERSE_H
