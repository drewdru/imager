#ifndef BINARIZATION_H
#define BINARIZATION_H

#include "binarization_global.h"

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

class PM_WIDGET_EXPORT Binarization : public ProcessImageInterface
{
public:
    Binarization();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void Binarize(uchar* bitmap, size_t n, int limit);
};

#endif // BINARIZATION_H
