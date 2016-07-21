#ifndef HISTOGRAMEQUALIZATION_H
#define HISTOGRAMEQUALIZATION_H

#include "histogramequalization_global.h"

#include <math.h>
#include <QVector>
#include <QString>
#include <QImage>

#define PLUGIN_VERSION 1.1

class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};

class PM_WIDGET_EXPORT HistogramEqualization : public ProcessImageInterface
{
public:
    HistogramEqualization();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void histogramEqualization(uchar *bitmap, size_t n, QSize imgSize);
};

#endif // HISTOGRAMEQUALIZATION_H
