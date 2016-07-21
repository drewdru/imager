#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQuick/QQuickImageProvider>
#include <QLibrary>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "ProcessImagePlugins.h"
#include "PluginInfo.h"
#include "PluginError.h"

#include <QDebug>

typedef ProcessPlugin* (*DLLObject)();
typedef void(*PlugInfo)(QJsonObject &json);
class ImageProcessor : public QObject
{
    Q_OBJECT
    QString libDirPath;
    QMap<QString, QJsonObject> pluginList; //ключ -- путь к плагину;
public:
    explicit ImageProcessor(QObject *parent = 0);
    ~ImageProcessor(){}
public slots:
    QString processImage(QString method, int arg);
    QStringList getMethodsList();
    void addDLL(QString path);
    QString pluginInfo(QString text);
};

#endif // IMAGEPROCESSOR_H
