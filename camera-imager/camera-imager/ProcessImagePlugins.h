#ifndef PROCESSIMAGEPLUGINS_H
#define PROCESSIMAGEPLUGINS_H
#include <QObject>
class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};
class ProcessPlugin : public ProcessImageInterface
{
};
#endif // PROCESSIMAGEPLUGINS_H

