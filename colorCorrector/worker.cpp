#include "worker.h"
#include "qdownloader.h"

Worker::Worker(QString cameraPath, QString cameraFolder)
{
	this->cameraPath = cameraPath;
	this->cameraFolder = cameraFolder;

	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));
	//timer->start(1000);
}

void Worker::doWork() {
	stopCapturing = false;
	unsigned int i = 0;
	while (true)
	{
		// qDebug()<<"infinity "<< i++;
		if (stopCapturing) break;
	}

	qDebug()<<"Worker is work!";

	//    emit resultReady(result);
}
void Worker::updateCaption()
{
	QUrl imageUrl(this->cameraPath);
	m_pImgCtrl = new FileDownloader(imageUrl, cameraFolder, this);
	emit process();
}

void Worker::StopPause()
{
	//timer->stop();
	if (timer->isActive()) {
		timer->stop();
	} else {
		timer->start(1000);
	}
}

void Worker::setCamPath(QString cameraPath)
{
	this->cameraPath = cameraPath;
}
