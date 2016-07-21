#ifndef GRAYWORLD_H
#define GRAYWORLD_H

#include "grayworld_global.h"
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

class PM_WIDGET_EXPORT GrayWorld : public ProcessImageInterface
{
public:
    GrayWorld();

    int Process(uchar* bitmap, size_t n, QVector<QString> argv);

private:
    void grayWorld(uchar* bitmap, size_t n, QSize imgSize);
};

#endif // GRAYWORLD_H
