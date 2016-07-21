#include "qdownloader.h"

#include <QPixmap>
FileDownloader::FileDownloader(QUrl imageUrl, QString cameraFolder, QObject *parent) :
	QObject(parent)
{
	connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
			this, SLOT (fileDownloaded(QNetworkReply*))
			);

//    QUrl imageUrl1("http://94.232.8.10/axis-cgi/jpg/image.cgi?camera=1");
	QNetworkRequest request(imageUrl);
	m_WebCtrl.get(request);

	this->cameraFolder = cameraFolder;
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* reply) {
	//m_DownloadedData = pReply->readAll();

	qDebug()<<"slot_netwManagerFinished";
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
		return;
	}
	QVariant attribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (attribute.isValid()) {
		QUrl url = attribute.toUrl();
		qDebug() << "must go to:" << url;
		return;
	}
	qDebug() << "ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(jpegData);
	pixmap.save(cameraFolder\
				+ QDateTime::currentDateTime().toString("dd.MM.yy hh.mm.ss")\
				+ ".bmp", "BMP");

	//emit a signal
	reply->deleteLater();
//    emit downloaded();
}
