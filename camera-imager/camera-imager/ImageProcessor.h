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
#include <QThread>
#include <QIODevice>
#include <QLocalServer>
#include <QMessageBox>
#include "Worker.h"

typedef ProcessPlugin* (*DLLObject)();
typedef void(*PlugInfo)(QJsonObject &json);
#include <QThread>
#include <QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
class ImageProcessor : public QObject
{
    Q_OBJECT
    QString libDirPath;
    QMap<QString, QJsonObject> pluginList; //ключ -- путь к плагину;
    Worker* workerCam1;
    Worker* workerCam2;
    QProcess* newProcess;

    QLocalServer *server;
    QStringList fortunes;

FileDownloader* m_pImgCtrl ;
public:
    explicit ImageProcessor(QObject *parent = 0);
    ~ImageProcessor();
public slots:
    QString processImage(QString method, double arg, int cbCurIndex);
    QStringList getMethodsList();
    void addDLL(QString path);
    QString pluginInfo(QString text);
    QString getCam1File();
    QString getCam2File();
    void spCam1();
    void spCam2();
	void processAllClicked(QString method, double arg, int cbCurIndex);
//    void startThread();

    void processOutput();
	void handleResults(const QString & result);

private slots:
    void sendFortune();

signals:
    void operate(const QString &);
};

#endif // IMAGEPROCESSOR_H
