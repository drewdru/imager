#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include "QDownloader.h"
#include <QTimer>
class Worker : public QObject
{
	Q_OBJECT
	FileDownloader* m_pImgCtrl;
	QTimer *timer;
	QString cameraPath;
	QString cameraFolder;
public:
	bool stopCapturing;
	Worker(QString cameraPath, QString cameraFolder);
	void StopPause();
	~Worker(){qDebug()<<"FUCK!";}
public slots:
	void doWork();
	void updateCaption();
	void setCamPath(QString cameraPath);
signals:
	void process();
};

#endif // WORKER_H
