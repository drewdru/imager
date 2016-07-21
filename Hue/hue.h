#ifndef HUE_H
#define HUE_H

#include "hue_global.h"


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

class PM_WIDGET_EXPORT Hue : public ProcessImageInterface
{
public:
    Hue();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void hue(uchar* bitmap, size_t n, int newH);
    void RGB_TO_HSL(uchar* bitmap, double* newBitmap, size_t n);
    void HSL_TO_RGB(uchar* bitmap, double* newBitmap, size_t n);
};
#endif // HUE_H
