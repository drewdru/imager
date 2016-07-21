#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<QString, int> threadList;
int COUNT = 0;

//class HelloWorldTask : public QRunnable
//{
//    void run()
//    {
//        qDebug() << "Hello world from thread" << QThread::currentThread();
//        QSqlQuery q;
//        q.exec("SELECT * FROM test1");
//        while(q.next()){
//            qDebug()<<QString::number(q.value("a").toInt());
//            qDebug()<<q.value("b").toString();
//        }
//    }
//};

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	qDebug()<<"TEST! stdOut from newProcess.";
	ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowTitle("Очередь обработки");
	dllPath.append(QApplication::arguments().at(2));
	ui->progressBar->hide();

	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("db.sqllite");
	if (!db.open()) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(db.lastError().text());
		msgBox.exec();
	}

	QSqlQuery a_query;
	// DDL query
	QString str = "CREATE TABLE IF NOT EXISTS test1(a INTEGER, b TEXT);";
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}
	str = "CREATE TABLE IF NOT EXISTS launches( "
	"id INTEGER PRIMARY KEY AUTOINCREMENT, "
	"datetext TEXT, "
	"threadid TEXT, "
	"method1 TEXT, "
	"method2 TEXT);";
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}
	str = "CREATE TABLE IF NOT EXISTS processing( "
	"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
	"state TEXT, "
	"imagepath TEXT, "
	"methodname TEXT, "
	"arg TEXT, "
	"cbCurIndex TEXT); ";
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}

	QString str_insert = "INSERT INTO launches(datetext, threadid, method1, method2) "
	"VALUES ('%1', '%2', '%3', '%4');";
	str = str_insert\
	.arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"))\
	.arg(QString::number((long long)QThread::currentThreadId(), 16))\
	.arg(QApplication::arguments().at(1))
	.arg(QApplication::arguments().at(3));
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}

	launchesModel = new QSqlTableModel(this,QSqlDatabase::database());
	launchesModel->setTable("launches");
	/*qDebug()<< */launchesModel->select();
	ui->launchesView->setModel(launchesModel);
	ui->launchesView->resizeColumnsToContents();

	processingModel = new QSqlTableModel(this,QSqlDatabase::database());
	processingModel->setTable("processing");
	/*qDebug()<< */processingModel->select();
	ui->processingView->setModel(processingModel);
	ui->processingView->resizeColumnsToContents();

	//    HelloWorldTask *hello = new HelloWorldTask();
	//    // QThreadPool takes ownership and deletes 'hello' automatically
	//    QThreadPool::globalInstance()->start(hello);

	blockSize = 0;
	socket = new QLocalSocket(this);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readFortune()));
	connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
	this, SLOT(displayError(QLocalSocket::LocalSocketError)));

	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));

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

	QMapIterator<QString, QJsonObject> i(pluginList);
	while (i.hasNext()) {
		i.next();
		ui->methodCBox1->addItem(i.key());
	}

}

void MainWindow::addDLL(QString path)
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

//void MainWindow::onSokReadyRead()
//{
//    qDebug()<<"Process get data from socket:\n"\
//           <<sokDataFromParent->readAll();
//    sokDataFromParent->write("NOoooooooooooooooooooooooooooooooooo!");
//}

MainWindow::~MainWindow()
{
	delete ui;
}
void incThreadList(QString threadID)
{
	threadList.contains(threadID)\
		?threadList[threadID]++\
		:threadList[threadID] = 1;
	++COUNT;
	qApp->processEvents();
}
void MainWindow::updateProgressBar()
{
	ui->progressBar->setValue(COUNT);
	qApp->processEvents();
}

PROCESSING process(const PROCESSING &proc)//, QString dllPath, int arg
{
	incThreadList(\
	QString::number((long long)QThread::currentThreadId(), 16));

	QLibrary lib(proc.methodPath);
	lib.load();
	DLLObject createObject = (DLLObject)lib.resolve("createObject");
	if (!createObject)
	{
		lib.unload();
		qDebug()<< "method createObject not found";
		return proc;
	}
	ProcessPlugin* myClassInstance = createObject();
	if(!myClassInstance)
	{
		lib.unload();
		qDebug()<<  "cannot create object";
		return proc;
	}

	QImage bufImage(proc.imagePath);
	QVector<QString> args;//
	//    if(!proc.arg.isEmpty())
	//        args.append(proc.arg);
	//    else
	//        args.append(QString::number(0));
	args.append(proc.arg);
	args.append(QString::number(bufImage.height()));
	args.append(QString::number(bufImage.width()));
	args.append(proc.cbCurIndex);

	qDebug()<< QString::number(myClassInstance->Process(bufImage.bits(),bufImage.byteCount(), args));
	bufImage.save("./image/" + proc.methodName + proc.imagePath.split("/").last());

	lib.unload();
	qApp->processEvents();
	qDebug() << "Scaling image in thread" << QThread::currentThread();

	QSqlQuery a_query;
	QString str = "UPDATE processing SET state = 'Done' "
	"WHERE id = "+ proc.id +";";
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}
	++COUNT;
	qApp->processEvents();

	return proc;//.scaled(QSize(100, 100), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void MainWindow::on_startProcessBtn_clicked()
{
	ui->startProcessBtn->setEnabled(false);
	ui->frame->setEnabled(false);
//	ui->standLabel->show();
	QSqlQuery a_query;
	// DDL query
	QString str = "SELECT * FROM processing WHERE state = 'Wait';";
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}

	QList<PROCESSING> procList;
	while (a_query.next())
	{
		if(pluginList[a_query.value("methodname")\
		.toString()]["pluginPath"].isNull())
			continue;
		PROCESSING proc;
		proc.id = a_query.value("id").toString();
		proc.state = a_query.value("state").toString();
		proc.imagePath = a_query.value("imagepath").toString();
		proc.methodName = a_query.value("methodname").toString();
		proc.methodPath = pluginList[a_query.value("methodname")\
			.toString()]["pluginPath"].toString();//a_query.value("methodpath").toString();
		proc.arg = a_query.value("arg").toString();
		proc.cbCurIndex = a_query.value("cbCurIndex").toString();
		procList.append(proc);
		qApp->processEvents();
	}

	COUNT = 0;
	ui->progressBar->show();
	ui->progressBar->setValue(0);
	ui->progressBar->setMaximum(procList.length()*2);
	timer->start(500);
	qApp->processEvents();

	QThreadPool::globalInstance()->setMaxThreadCount(ui->threadNumSB->value()-1);//n-1 (n[min]=2)
	// Use QtConcurrentBlocking::mapped to apply the scale function to all the
	// images in the list.
	QList<PROCESSING> thumbnails = QtConcurrent::blockingMapped< QList<PROCESSING> >(procList, process);

//	for (int i = 0; i < thumbnails.size(); ++i) {
//		QString str = "UPDATE processing SET state = 'Done' "
//		"WHERE id = "+ thumbnails.at(i).id +";";
//		if (!a_query.exec(str)) {
//			QMessageBox msgBox;
//			msgBox.setText(a_query.lastError().text());
//			msgBox.exec();
//		}
//		++COUNT;
//		qApp->processEvents();
//	}
	timer->stop();
	qApp->processEvents();

	qDebug()<< processingModel->select();
	ui->processingView->setModel(processingModel);

	QString threadCounStr;
	QMapIterator<QString, int> i(threadList);
	while (i.hasNext()) {
		i.next();
		threadCounStr.append("Thread " + i.key()\
		+ " Run "+QString::number(i.value())\
		+" tasks\n");
		qApp->processEvents();
	}
	QMessageBox msgBox;
	msgBox.setWindowTitle("Complete");
	msgBox.setText(threadCounStr);
	msgBox.exec();
	ui->startProcessBtn->setEnabled(true);
	ui->frame->setEnabled(true);
	ui->progressBar->hide();
}

void MainWindow::on_browseBtn_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this,
	tr("Open Image"), "media/drew/LocalDisk5/QtDocuments/projectsqt/imager/build-imager-Desktop_Qt_5_5_0_GCC_64bit-Debug/camera-imager",\
	tr("Image Files (*.png *.jpg *.bmp)"));
	ui->filePathEL->setText(filePath);
}

void MainWindow::on_browseDirBtn_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
	"media/drew/LocalDisk5/QtDocuments/projectsqt/imager/build-imager-Desktop_Qt_5_5_0_GCC_64bit-Debug/camera-imager",
	QFileDialog::ShowDirsOnly
	| QFileDialog::DontResolveSymlinks);
	ui->dirPathEL->setText(dir);
}

void MainWindow::on_insertFileBtn_clicked()
{
	if(ui->filePathEL->text().isEmpty())
		return;
	QString method = ui->methodCBox1->currentText();
	double arg = ui->valueSBox1->value();
	int cbCurIndex = ui->modeCBox1->currentIndex();
	QString str_insert = "INSERT INTO processing(state, imagepath, methodname, arg, cbCurIndex) "
	"VALUES ('%1', '%2', '%3', '%4', '%5');";
	QString str = str_insert\
		.arg("Wait")\
		.arg(ui->filePathEL->text())
		.arg(method)\
		.arg(arg)\
		.arg(cbCurIndex);
	QSqlQuery a_query;
	if (!a_query.exec(str)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(a_query.lastError().text());
		msgBox.exec();
	}
	qDebug()<< processingModel->select();
	ui->processingView->setModel(processingModel);
	ui->filePathEL->clear();
}

void MainWindow::on_insertDirBtn_clicked()
{
	QString method = ui->methodCBox1->currentText();
	double arg = ui->valueSBox1->value();
	int cbCurIndex = ui->modeCBox1->currentIndex();

	if(ui->dirPathEL->text().isEmpty())
		return;
	QDir dir;
	dir.setPath(ui->dirPathEL->text());
	QList<QFileInfo> fileList;
	QStringList filter;

	//    //TODO:  change file filter
	//    filter<<"*.jpeg"<<"*.jpg"<<"*.bmp"";
	fileList = dir.entryInfoList(filter);

	ui->frame->setEnabled(false);
	ui->progressBar->show();
	ui->progressBar->setValue(0);
	ui->progressBar->setMaximum(fileList.size());
	for (int i = 0; i < fileList.size(); ++i)
	{
		QString str_insert = "INSERT INTO processing(state, imagepath, methodname, arg, cbCurIndex) "
		"VALUES ('%1', '%2', '%3', '%4', '%5');";
		QString str = str_insert\
		.arg("Wait")\
		.arg(fileList.at(i).filePath())
		.arg(method)\
		.arg(arg)\
		.arg(cbCurIndex);
		QSqlQuery a_query;
		if (!a_query.exec(str)) {
			QMessageBox msgBox;
			msgBox.setWindowTitle("Error");
			msgBox.setText(a_query.lastError().text());
			msgBox.exec();
		}
		ui->progressBar->setValue(ui->progressBar->value()+1);
		qApp->processEvents();
	}
	//get Dynamic Link Library files
	qDebug()<< processingModel->select();
	ui->processingView->setModel(processingModel);
	ui->progressBar->hide();
	ui->frame->setEnabled(true);
	QMessageBox msgBox;
	msgBox.setWindowTitle("Complete!");
	msgBox.setText("Complete!");
	msgBox.exec();
	ui->dirPathEL->clear();
}

void MainWindow::on_pushButton_clicked()
{
	blockSize = 0;
	socket->abort();
	socket->connectToServer("myserver");
}


void MainWindow::readFortune()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_5_5);

	if (blockSize == 0) {
		if (socket->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> blockSize;
	}

	if (in.atEnd())
		return;

	QString nextFortune;
	in >> nextFortune;

	if (nextFortune == currentFortune) {
		QTimer::singleShot(0, this, SLOT(requestNewFortune()));
		return;
	}

	currentFortune = nextFortune;
	QMessageBox answer;
	answer.setWindowTitle("Server response");
	answer.setText(currentFortune);
	answer.exec();
}

void MainWindow::displayError(QLocalSocket::LocalSocketError socketError)
{
	QMessageBox inf;
	inf.setWindowTitle("ERROR!");
	switch (socketError) {
	case QLocalSocket::ServerNotFoundError:
		inf.setText(tr("The host was not found. Please check the "\
		"host name and port settings."));
		inf.exec();
		break;
	case QLocalSocket::ConnectionRefusedError:
		inf.setText(tr("The connection was refused by the peer. "\
		"Make sure the fortune server is running, "\
		"and check that the host name and port "\
		"settings are correct."));
		inf.exec();
		break;
	case QLocalSocket::PeerClosedError:
		break;
	default:
		inf.setText(socket->errorString());
		inf.exec();
		break;
	}
}

QString MainWindow::pluginInfo(QString text)
{
	QJsonDocument doc(pluginList[text]["arguments"].toArray());
	QString arg = doc.toJson(QJsonDocument::Compact);
	QStringList argLst = arg.split(",");
	arg.clear();
	for(int i = 0; i< argLst.length(); i++)
	{
		arg.append(argLst.at(i));
		arg.append(", ");
//		if(i==argLst.length()/2 && argLst.length()>4)
//			arg.append("\n");
	}
	QString str = "Plugin: " + text + " v"\
	+ QString::number(pluginList[text]["version"].toDouble())\
	+ " Author: " + pluginList[text]["authorInfo"].toString()\
	+ " Arguments:" + arg;
	return str;
}

void MainWindow::on_methodCBox1_currentTextChanged(const QString &arg1)
{
	ui->methodCBox1->blockSignals(true);
	ui->meth1->setText(arg1);
//	ui->info1->setText(pluginInfo(arg1));
	QJsonDocument doc(pluginList[arg1]["arguments"].toArray());
	QString response = doc.toJson(QJsonDocument::Compact);
	qApp->processEvents();

	QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
	QJsonArray jsonArr = jsonResponse.array();
	QJsonArray::iterator i;
	for (i = jsonArr.begin(); i != jsonArr.end(); ++i)
	{
		QJsonObject obj = (*i).toObject();
		if(obj["name"].toString() == "") {
			//ui->frameConfig->show();
			ui->meth1->hide();
			ui->valueSBox1->hide();

			ui->mode1->hide();
			ui->modeCBox1->hide();
			break;
		}
		else {
			//ui->frameConfig->show();

			ui->meth1->hide();
			ui->valueSBox1->hide();

			ui->mode1->hide();
			ui->modeCBox1->hide();
			ui->modeCBox1->clear();
		}
		if(obj["type"].toString() == "slider")
		{
			ui->meth1->setText(obj["name"].toString());
			ui->valueSBox1->setMinimum(obj["min"].toString().toInt());
			ui->valueSBox1->setMaximum(obj["max"].toString().toInt());

			if(obj["step"].toString() == "")
				ui->valueSBox1->setSingleStep(1.0);
			else
				ui->valueSBox1->setSingleStep(obj["step"].toString().toDouble());


			ui->meth1->show();
			ui->valueSBox1->show();
		}
		if(obj["type"].toString() == "cbox")
		{
			ui->mode1->setText(obj["name"].toString());
			for(int j = 0; j< obj["count"].toString().toInt();j++)
			{
				QString ind = "var"+QString::number(j);
				ui->modeCBox1->addItem(obj[ind].toString());
			}

			ui->mode1->show();
			ui->modeCBox1->show();
		}
	}
	ui->methodCBox1->blockSignals(false);
}
