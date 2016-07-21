#include "ImageProcessor.h"
#include <QDir>
#include <QDateTime>
#include <QFileDialog>

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
{
    QDir dir;
    libDirPath = dir.absolutePath()+ QDir::separator() + "lib" + QDir::separator();
    dir.setPath("./lib");

    //get Dynamic Link Library files
    QList<QFileInfo> fileList;
    QStringList filter;

    //TODO:  change file type for OS
    filter<<"*.so"<<"*.dll"<<"*.dylib"<<"*.sl"<<"*.bundle";
    fileList = dir.entryInfoList(filter);

    //load libs
    for (int i = 0; i < fileList.size(); ++i)
    {
        addDLL(fileList.at(i).filePath());
    }
}

QStringList ImageProcessor::getMethodsList()
{
    return pluginList.keys();
}

QString ImageProcessor::processImage(QString method, int arg)
{
    QLibrary lib(pluginList[method]["pluginPath"].toString());
    lib.load();
    DLLObject createObject = (DLLObject)lib.resolve("createObject");
    if (!createObject)
    {
        lib.unload();
        return "method createObject not found";
    }
    ProcessPlugin* myClassInstance = createObject();
    if(!myClassInstance)
    {
        lib.unload();
        return "cannot create object";
    }
    QImage image("file.bmp");//image.bits()->BGRA
    QVector<QString> args;//
    args.append(QString::number(arg));
    qDebug()<< myClassInstance->Process(image.bits(),image.byteCount(), args);
    image.save("testOUT.bmp");
    lib.unload();

    QJsonDocument doc(pluginList[method]["arguments"].toArray());
    qDebug()<<doc.toJson(QJsonDocument::Compact);
    return doc.toJson(QJsonDocument::Compact);
}

QString ImageProcessor::pluginInfo(QString text)
{
    QJsonDocument doc(pluginList[text]["arguments"].toArray());
    QString str = "Plugin: " + text + "v"\
            + pluginList[text]["version"].toString()\
            + "\nAuthor: " + pluginList[text]["authorInfo"].toString()\
            + "\nArguments: " + doc.toJson(QJsonDocument::Compact);
    return str;
}

void ImageProcessor::addDLL(QString path)
{
    QLibrary lib(path);
    if (!lib.load())
    {
        qDebug()<<lib.errorString();
        return;
    }
    PlugInfo plugInfo = (PlugInfo)lib.resolve("PluginInfo");
    if (!plugInfo)
    {
        lib.unload();
        qDebug()<<"PluginInfo not found";
        return;
    }
    QJsonObject info;
    plugInfo(info);
    if (pluginList.contains(info["pluginName"].toString()))
        if(info["version"].toDouble() <= pluginList[info["pluginName"].toString()]["version"].toDouble())
        {
            lib.unload();
            return;
        }
    info["pluginPath"] = path;
    pluginList.insert(info["pluginName"].toString(),info);
    lib.unload();
}

