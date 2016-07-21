#ifndef GAMMA_H
#define GAMMA_H

#include <math.h>
#include <QVector>
#include <QString>
#include "gamma_global.h"

#define PLUGIN_VERSION 1.0

class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};

class PM_WIDGET_EXPORT Gamma : public ProcessImageInterface
{
public:
    Gamma();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void gamma(uchar* bitmap, size_t n, float value);
};

#endif // GAMMA_H
