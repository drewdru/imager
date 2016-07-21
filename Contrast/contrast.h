#ifndef CONTRAST_H
#define CONTRAST_H

#include "contrast_global.h"


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

class PM_WIDGET_EXPORT Contrast : public ProcessImageInterface
{
public:
    Contrast();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void contrast(uchar* bitmap, size_t n, int threshold);
};


#endif // CONTRAST_H
