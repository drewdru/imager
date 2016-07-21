#ifndef GRAY_H
#define GRAY_H

#include "gray_global.h"

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

class PM_WIDGET_EXPORT Gray : public ProcessImageInterface
{
public:
    Gray();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void gray(uchar* bitmap, size_t n);
};

#endif // GRAY_H
