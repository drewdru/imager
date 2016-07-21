#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowTitle("Система обработки видеопоследовательностей");
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
	QString camPath = ui->lineEdit->text();
	workerCam1 = new Worker(camPath,
	"./firstCam/");
	QtConcurrent::run(workerCam1,&Worker::doWork);
	connect(workerCam1, SIGNAL(process()), this, SLOT(updateCam1()));

	camPath = "http://http://94.232.8.10/axis-cgi/jpg/image.cgi";
	workerCam2 = new Worker(camPath,
	"./secondCam/");
	QtConcurrent::run(workerCam2,&Worker::doWork);
	connect(workerCam2, SIGNAL(process()), this, SLOT(updateCam2()));

	newProcess = new QProcess;

	server = new QLocalServer(this);
	if (!server->listen("myserver")) {
		QMessageBox critical;
		critical.setWindowTitle("Error");
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


	pixMap1.load("cam1.bmp");
	ui->image1->setPixmap(pixMap1);
	pixMap2.load("cam2.bmp");
//	ui->image2->setPixmap(pixMap2);

	QMapIterator<QString, QJsonObject> i(pluginList);
	while (i.hasNext()) {
		i.next();
		ui->methodCBox1->addItem(i.key());
//		ui->methodCBox2->addItem(i.key());
	}

//	ui->info1->hide();
//	ui->frameConfig->hide();
//	ui->frameConfig_2->hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
//	ui->frameConfig->show();
//	ui->frameConfig_2->show();
	event->accept();
}

MainWindow::~MainWindow()
{
	workerCam1->stopCapturing = true;
	workerCam2->stopCapturing = true;
	workerCam1->deleteLater();
	workerCam2->deleteLater();
	delete ui;
}

void MainWindow::handleResults(const QString &result)
{
	//qDebug()<<result;
}

QString MainWindow::getCam1File()
{
	QDir dir;
	dir.setPath("./firstCam");
	QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time);
	if(!list.isEmpty())
		return "./firstCam/" + list.first().fileName();
	else
		return "./firstCam/";
}

QString MainWindow::getCam2File()
{
	QDir dir;
	dir.setPath("./secondCam");
	QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDir::Time);
	if(!list.isEmpty())
		return "./secondCam/" + list.first().fileName();
	else
		return "./secondCam/";
}

void MainWindow::spCam1()
{
	workerCam1->StopPause();
}

void MainWindow::spCam2()
{
	workerCam2->StopPause();
}

QStringList MainWindow::getMethodsList()
{
	return pluginList.keys();
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

// this gets called whenever the process has something to say...
void MainWindow::processOutput()
{
	qDebug() << newProcess->readAllStandardOutput();  // read normal output
	qDebug() << newProcess->readAllStandardError();  // read error channel
}

void MainWindow::sendFortune()
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

void MainWindow::on_processAllButton_clicked()
{
	QString method = ui->methodCBox1->currentText();
//	QString method2 = ui->methodCBox2->currentText();
//	double arg = ui->valueSBox1->value();
//	int cbCurIndex = ui->modeCBox1->currentIndex();
	newProcess->close();
	newProcess->deleteLater();
	qApp->processEvents();

	QString program = "./packageCorrector";

	QStringList arguments;
//	arguments << method << pluginList[method]["pluginPath"].toString() << method2;//<< QString::number(arg) << QString::number(cbCurIndex);
	newProcess = new QProcess;
	connect (newProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));  // connect process signals with your code
	connect (newProcess, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));  // same here
	newProcess->setProcessChannelMode(QProcess::MergedChannels);

	newProcess->start(program, arguments);
	newProcess->write("TEST WRITING TO NEW PROCESS!",300);
	//newProcess->waitForBytesWritten(500);

}

void MainWindow::on_addDLLButton_clicked()
{
	QString meth1 = ui->methodCBox1->currentText();
//	QString meth2 = ui->methodCBox2->currentText();
	ui->methodCBox1->blockSignals(true);
//	ui->methodCBox2->blockSignals(true);
	QString filePath = QFileDialog::getOpenFileName(this,
	tr("Open DLL"), "./",\
	tr("DLL files (*.so *.dll *.dylib *.sl *.bundle)"));
	addDLL(filePath);
	ui->methodCBox1->clear();
//	ui->methodCBox2->clear();
	QMapIterator<QString, QJsonObject> i(pluginList);
	while (i.hasNext()) {
		i.next();
		ui->methodCBox1->addItem(i.key());
//		ui->methodCBox2->addItem(i.key());
	}
	ui->methodCBox1->setCurrentText(meth1);
//	ui->methodCBox2->setCurrentText(meth2);
	ui->methodCBox1->blockSignals(false);
//	ui->methodCBox2->blockSignals(false);
	QFile::copy(filePath, "./lib/" + filePath.split("/").last());
}

void MainWindow::on_sPButton1_clicked()
{
	workerCam1->StopPause();
	ui->sPButton1->setText(ui->sPButton1->text() == "Запуск"\
		? "Пауза" \
		: "Запуск");
}

void MainWindow::on_sPButton2_clicked()
{
	workerCam2->StopPause();
/*	ui->sPButton2->setText(ui->sPButton2->text() == "Запуск"\
		? "Пауза" \
		: "Запуск");*/
}

QString MainWindow::updateCam1()
{
	QString method = ui->methodCBox1->currentText();
	double arg = ui->valueSBox1->value();
	int cbCurIndex = ui->modeCBox1->currentIndex();

	QLibrary lib(pluginList[method]["pluginPath"].toString());
	lib.load();
	DLLObject createObject = (DLLObject)lib.resolve("createObject");
	if (!createObject)
	{
		lib.unload();
		qDebug()<< "method createObject not found";
	}
	ProcessPlugin* myClassInstance = createObject();
	if(!myClassInstance)
	{
		lib.unload();
		qDebug()<< "cannot create object";
	}

	//qDebug()<<getCam1File();
	QImage image(getCam1File());//image.bits()->BGRA
	image.save("cam2.bmp");
	pixMap2.load("cam2.bmp");
	ui->image2->setPixmap(pixMap2);

	QVector<QString> args;//
	args.append(QString::number(arg));
	args.append(QString::number(image.size().height()));
	args.append(QString::number(image.size().width()));
	args.append(QString::number(cbCurIndex));

//	QTime timer;
//	timer.start();
	/*qDebug()<< */myClassInstance->Process(image.bits(),image.byteCount(), args);
//	int time = timer.elapsed();
//	QFile file("./log.txt");
//	if (file.open(QIODevice::Append)) {
//	   file.write(method.toUtf8()+":\t" + QString::number(time).toUtf8() + "\n");
//	}
//	file.close();

	image.save("cam1.bmp");

	lib.unload();

	pixMap1.load("cam1.bmp");
	ui->image1->setPixmap(pixMap1);
//	pixMap2.load("cam2.bmp");
//	ui->image2->setPixmap(pixMap2);

	QJsonDocument doc(pluginList[method]["arguments"].toArray());
	//    qDebug()<<doc.toJson(QJsonDocument::Compact);
	return doc.toJson(QJsonDocument::Compact);
}

QString MainWindow::updateCam2()
{
/*	QString method = ui->methodCBox2->currentText();
	double arg = ui->valueSBox2->value();
	int cbCurIndex = ui->modeCBox2->currentIndex();*/

/*	QLibrary lib(pluginList[method]["pluginPath"].toString());
	lib.load();
	DLLObject createObject = (DLLObject)lib.resolve("createObject");
	if (!createObject)
	{
		lib.unload();
		qDebug()<< "method createObject not found";
	}
	ProcessPlugin* myClassInstance = createObject();
	if(!myClassInstance)
	{
		lib.unload();
		qDebug()<< "cannot create object";
	}

	//qDebug()<<getCam1File();
	QImage image(getCam2File());//image.bits()->BGRA
	QVector<QString> args;//
	args.append(QString::number(arg));
	args.append(QString::number(image.size().height()));
	args.append(QString::number(image.size().width()));
	args.append(QString::number(cbCurIndex));
	/*qDebug()<< *//*myClassInstance->Process(image.bits(),image.byteCount(), args);
	image.save("cam2.bmp");

	lib.unload();

	pixMap2.load("cam2.bmp");
//	ui->image2->setPixmap(pixMap2);

	QJsonDocument doc(pluginList[method]["arguments"].toArray());
	//    qDebug()<<doc.toJson(QJsonDocument::Compact);
	return doc.toJson(QJsonDocument::Compact);*/
}

void MainWindow::on_lineEdit_editingFinished()
{
//	workerCam1->setCamPath("http://" \
//		+ ui->lineEdit->text() \
//		+ "/axis-cgi/jpg/image.cgi?camera=1");
	workerCam1->setCamPath(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_2_editingFinished()
{
/*	workerCam2->setCamPath("http://" \
		+ ui->lineEdit_2->text() \
		+ "/axis-cgi/jpg/image.cgi?camera=1");*/
}

void MainWindow::on_methodCBox1_currentTextChanged(const QString &arg1)
{
	ui->methodCBox1->blockSignals(true);
	ui->meth1->setText(arg1);
//	ui->info1->setText(pluginInfo(arg1));
	QString response = updateCam1();
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

void MainWindow::on_methodCBox2_currentTextChanged(const QString &arg1)
{
/*	ui->methodCBox2->blockSignals(true);
	ui->meth2->setText(arg1);
	ui->info2->setText(pluginInfo(arg1));*/
/*	QString response = updateCam2();
	qApp->processEvents();

	QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
	QJsonArray jsonArr = jsonResponse.array();
	QJsonArray::iterator i;
	for (i = jsonArr.begin(); i != jsonArr.end(); ++i)
	{
		QJsonObject obj = (*i).toObject();
		if(obj["name"].toString() == "") {
			//ui->frameConfig->show();
/*			ui->meth2->hide();
			ui->valueSBox2->hide();

			ui->mode2->hide();
			ui->modeCBox2->hide();
			break;
		}
		else {
			//ui->frameConfig->show();

			ui->meth2->hide();
			ui->valueSBox2->hide();

			ui->mode2->hide();
			ui->modeCBox2->hide();
			ui->modeCBox2->clear();
		}
		if(obj["type"].toString() == "slider")
		{
			ui->meth2->setText(obj["name"].toString());
			ui->valueSBox2->setMinimum(obj["min"].toString().toInt());
			ui->valueSBox2->setMaximum(obj["max"].toString().toInt());

			if(obj["step"].toString() == "")
				ui->valueSBox2->setSingleStep(1.0);
			else
				ui->valueSBox2->setSingleStep(obj["step"].toString().toDouble());


			ui->meth2->show();
			ui->valueSBox2->show();
		}
		if(obj["type"].toString() == "cbox")
		{
			ui->mode2->setText(obj["name"].toString());
			for(int j = 0; j< obj["count"].toString().toInt();j++)
			{
				QString ind = "var"+QString::number(j);
				ui->modeCBox2->addItem(obj[ind].toString());
			}

			ui->mode2->show();
			ui->modeCBox2->show();
		}
	}
	ui->methodCBox2->blockSignals(false);*/
}

void MainWindow::on_valueSBox1_editingFinished()
{
	updateCam1();
}

void MainWindow::on_modeCBox1_currentIndexChanged(int index)
{
	if(index>-1)
		updateCam1();
}

void MainWindow::on_valueSBox2_editingFinished()
{
	updateCam2();
}

void MainWindow::on_modeCBox2_currentIndexChanged(int index)
{
	if(index>-1)
		updateCam2();
}

void MainWindow::on_ConfigButton1_clicked()
{
/*	if(ui->ConfigButton1->text() == "Отобразить настройки")
	{
		ui->ConfigButton1->setText("Скрыть настройки");
		ui->frameConfig->show();
	}
	else
	{
		ui->ConfigButton1->setText("Отобразить настройки");
		ui->frameConfig->hide();
	}*/
}

void MainWindow::on_ConfigButton2_clicked()
{
/*	if(ui->ConfigButton2->text() == "Отобразить настройки")
	{
		ui->ConfigButton2->setText("Скрыть настройки");
		ui->frameConfig_2->show();
	}
	else
	{
		ui->ConfigButton2->setText("Отобразить настройки");
		ui->frameConfig_2->hide();
	}*/
}

void MainWindow::on_pushButton_clicked()
{
	QDir dir;
	libDirPath = dir.absolutePath()+ QDir::separator() + "lib" + QDir::separator();
	dir.setPath("./image");

	//get Dynamic Link Library files
	QList<QFileInfo> fileList;
	QStringList filter;

	//TODO:  change file type for OS
	filter<<"*.bmp";
	fileList = dir.entryInfoList(filter);

	//load libs
	for (int i = 0; i < fileList.size(); ++i)
	{
		QImage test(fileList.at(i).filePath());
		uchar* bitmap = test.bits();
		size_t n = test.byteCount();
		for (size_t i = 1; i < n-1; i+=4)
		{
			double Y = 0.299 * bitmap[i - 1] + 0.587 * bitmap[i] + 0.114 * bitmap[i+1];
			double U = -0.14713 * bitmap[i - 1] - 0.28886 * bitmap[i] + 0.436 * bitmap[i + 1] + 128;
			double V = 0.615 * bitmap[i - 1] - 0.51499 * bitmap[i] - 0.10001 * bitmap[i + 1] + 128;
			if (Y>=0&&Y<=255) bitmap[i - 1] = Y;
			else if (Y<0) bitmap[i - 1] = 0;
			else bitmap[i - 1] = 255;
			if (U>=0&&U<=255) bitmap[i] = U;
			else if (U<0) bitmap[i] = 0;
			else bitmap[i] = 255;
			if (V>=0&&V<=255) bitmap[i + 1] = V;
			else if (V<0) bitmap[i + 1] = 0;
			else bitmap[i + 1] = 255;
		}
		//bitmap[i - 1]
		size_t N = test.width()*test.height();
		int k = 256;
		int* histogrammY = new int[k];
		for (int i = 0; i<k; i++)
		{
			histogrammY[i] = 0;
		}

		for (size_t i = 1; i < n-1; i+=4)
		{
			++histogrammY[(int)bitmap[i - 1]];
		}
		QString testStr;
		testStr.append(fileList.at(i).filePath() + "\n");
		for (int i = 0; i<k; i++)
		{
			testStr.append(QString::number(histogrammY[i]) + "\n");
		}
		QFile file("./log.txt");
		if (file.open(QIODevice::Append)) {
			file.write(testStr.toUtf8());
		}
		file.close();
		delete[] histogrammY;
	}
}
