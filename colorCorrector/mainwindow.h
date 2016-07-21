#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>
#include "qdownloader.h"

#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include <qlocalserver.h>
#include <qlocalsocket.h>

#include <QObject>
#include <QLibrary>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "processimageplugins.h"
//#include "PluginInfo.h"
//#include "PluginError.h"

#include <QDebug>
#include <QTimer>
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

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QString libDirPath;
	QMap<QString, QJsonObject> pluginList; //ключ -- путь к плагину;
	Worker* workerCam1;
	Worker* workerCam2;
	QProcess* newProcess;

	QTimer *timer;

	QLocalServer *server;
	QStringList fortunes;

	FileDownloader* m_pImgCtrl ;
	QPixmap pixMap1;
	QPixmap pixMap2;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

public slots:
//	QString processImage();
	QStringList getMethodsList();
	void addDLL(QString path);
	QString pluginInfo(QString text);
	QString getCam1File();
	QString getCam2File();
	void spCam1();
	void spCam2();
	QString updateCam1();
	QString updateCam2();
//    void startThread();

	void processOutput();
	void handleResults(const QString & result);

private slots:
	void closeEvent(QCloseEvent *event);

	void sendFortune();

	void on_processAllButton_clicked();

	void on_addDLLButton_clicked();

	void on_sPButton1_clicked();

	void on_sPButton2_clicked();

	void on_lineEdit_editingFinished();

	void on_lineEdit_2_editingFinished();

	void on_methodCBox1_currentTextChanged(const QString &arg1);

	void on_methodCBox2_currentTextChanged(const QString &arg1);

	void on_valueSBox1_editingFinished();

	void on_modeCBox1_currentIndexChanged(int index);

	void on_valueSBox2_editingFinished();

	void on_modeCBox2_currentIndexChanged(int index);

	void on_ConfigButton1_clicked();

	void on_ConfigButton2_clicked();

	void on_pushButton_clicked();

signals:
	void operate(const QString &);
};

#endif // MAINWINDOW_H
