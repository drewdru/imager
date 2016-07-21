#include "ImageProcessor.h"
#include <QDir>
#include <QDateTime>
#include <QFileDialog>
#include "QDownloader.h"

#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include <qlocalserver.h>
#include <qlocalsocket.h>

//#include <turbojpeg.h>
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

    /* http://webocam.ru/axisapi */
	QString camPath = "http://94.232.8.10/axis-cgi/jpg/image.cgi?camera=1";
	workerCam1 = new Worker(camPath,
                            "./StreamAMAKSHotelWEBOCAM/");
    QtConcurrent::run(workerCam1,&Worker::doWork);
	camPath = "http://212.142.228.68/axis-cgi/jpg/image.cgi?camera=1";
	workerCam2 = new Worker(camPath,
                            "./StreamJapansSeaWEBOCAM/");
    QtConcurrent::run(workerCam2,&Worker::doWork);

    newProcess = new QProcess;

    server = new QLocalServer(this);
    if (!server->listen("myserver")) {
        QMessageBox critical;
        critical.setText(server->errorString());
        critical.exec();
        server->close();
        return;
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(sendFortune()));

    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");


}

ImageProcessor::~ImageProcessor()
{
	workerCam1->stopCapturing = true;
	workerCam2->stopCapturing = true;
	workerCam1->deleteLater();
	workerCam2->deleteLater();
}

void ImageProcessor::handleResults(const QString &result)
{
    //qDebug()<<result;
}

QString ImageProcessor::getCam1File()
{
    QDir dir;
    dir.setPath("./StreamAMAKSHotelWEBOCAM");
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time);
    if(!list.isEmpty())
        return "./StreamAMAKSHotelWEBOCAM/" + list.first().fileName();
    else
        return "./StreamAMAKSHotelWEBOCAM/";
}

QString ImageProcessor::getCam2File()
{
    QDir dir;
    dir.setPath("./StreamJapansSeaWEBOCAM");
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time);
    if(!list.isEmpty())
        return "./StreamJapansSeaWEBOCAM/" + list.first().fileName();
    else
        return "./StreamJapansSeaWEBOCAM/";
}

void ImageProcessor::spCam1()
{
    workerCam1->StopPause();
}

void ImageProcessor::spCam2()
{
    workerCam2->StopPause();
}

QStringList ImageProcessor::getMethodsList()
{
    return pluginList.keys();
}

QString ImageProcessor::processImage(QString method, double arg, int cbCurIndex)
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

    //qDebug()<<getCam1File();
    QImage image(getCam1File());//image.bits()->BGRA
    QVector<QString> args;//
    args.append(QString::number(arg));
    args.append(QString::number(image.size().height()));
    args.append(QString::number(image.size().width()));
    args.append(QString::number(cbCurIndex));
    /*qDebug()<< */myClassInstance->Process(image.bits(),image.byteCount(), args);
    image.save("cam1.bmp");

	qApp->processEvents();

    //qDebug()<<getCam2File();
    QImage image2(getCam2File());//image.bits()->BGRA
    QVector<QString> args2;//
    args2.append(QString::number(arg));
    args2.append(QString::number(image2.size().height()));
    args2.append(QString::number(image2.size().width()));
    args2.append(QString::number(cbCurIndex));
    /*qDebug()<< */myClassInstance->Process(image2.bits(),image2.byteCount(), args2);
    image2.save("cam2.bmp");

    lib.unload();

    QJsonDocument doc(pluginList[method]["arguments"].toArray());
    //    qDebug()<<doc.toJson(QJsonDocument::Compact);
    return doc.toJson(QJsonDocument::Compact);
}

QString ImageProcessor::pluginInfo(QString text)
{
    QJsonDocument doc(pluginList[text]["arguments"].toArray());
    QString arg = doc.toJson(QJsonDocument::Compact);
    QStringList argLst = arg.split(",");
    arg.clear();
    for(int i = 0; i< argLst.length(); i++)
    {
        arg.append(argLst.at(i));
        arg.append(",");
        if(i==argLst.length()/2 && argLst.length()>4)
            arg.append("\n");
    }
    QString str = "Plugin: " + text + " v"\
            + QString::number(pluginList[text]["version"].toDouble())\
            + "\nAuthor: " + pluginList[text]["authorInfo"].toString()\
            + "\nArguments:" + arg;
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

void ImageProcessor::processAllClicked(QString method, double arg, int cbCurIndex)
{
    newProcess->close();
    newProcess->deleteLater();
    qApp->processEvents();

	QString program = "./lab4";

    QStringList arguments;
	arguments << method << pluginList[method]["pluginPath"].toString() << QString::number(arg) << QString::number(cbCurIndex);
    newProcess = new QProcess;
    connect (newProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));  // connect process signals with your code
    connect (newProcess, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));  // same here
    newProcess->setProcessChannelMode(QProcess::MergedChannels);

    newProcess->start(program, arguments);
    newProcess->write("TEST WRITING TO NEW PROCESS!",300);
	//newProcess->waitForBytesWritten(500);
}

// this gets called whenever the process has something to say...
void ImageProcessor::processOutput()
{
    qDebug() << newProcess->readAllStandardOutput();  // read normal output
    qDebug() << newProcess->readAllStandardError();  // read error channel
}

void ImageProcessor::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
}
