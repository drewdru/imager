#ifndef CHANGEBRIGHTNESS_H
#define CHANGEBRIGHTNESS_H

#include "changebrightness_global.h"

#include <math.h>
#include <QVector>
#include <QString>

#define PLUGIN_VERSION 1.1

class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};

class PM_WIDGET_EXPORT ChangeBrightness : public ProcessImageInterface
{
public:
    ChangeBrightness();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void RGBToYUV(uchar *bitmap, size_t n);
    void YUVToRGB(uchar* bitmap, size_t n);
    void ChangeBrightnessY(uchar* bitmap, size_t n, uchar Y);
};

#endif // CHANGEBRIGHTNESS_H
