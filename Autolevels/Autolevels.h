#ifndef AUTOLEVELS_H
#define AUTOLEVELS_H

#include "autolevels_global.h"

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

class PM_WIDGET_EXPORT Autolevels : public ProcessImageInterface
{
public:
    Autolevels();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void autolevels(uchar* bitmap, size_t n);
};

#endif // AUTOLEVELS_H
