#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

#include <QObject>
#include <QLibrary>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QDebug>
#include <QtSql>
#include <QImage>
#include <QList>
#include <QThread>
#include <QDebug>
#include <QGuiApplication>
#include <qtconcurrentmap.h>
#include <QThreadPool>
#include <QFileDialog>
#include <QMessageBox>
#include <QLocalSocket>

class PluginInfo
{
    QString pluginPath;
    QString arguments;
    QString authorInfo;
    float version;    

public:
    PluginInfo();
    PluginInfo(QString pluginPath, QString arguments, QString authorInfo, float version);
    QString getPluginPath();
    QString getArguments();
    QString getAuthorInfo();
    float getVersion();

//    friend ostream &operator<<(ostream &stream, Trigon obj);//потоковый вывод экземпляра
//    friend istream &operator>>(istream &stream, Trigon &obj);//потоковый ввод экземпляра
};
class ProcessImageInterface
{
public:
    virtual ~ProcessImageInterface(){}
    virtual int Process(uchar* bitmap, size_t n, QVector<QString> argv) = 0;
};
class ProcessPlugin : public ProcessImageInterface
{
};
typedef ProcessPlugin* (*DLLObject)();
typedef void(*PlugInfo)(QJsonObject &json);

typedef struct {
	QString id;
	QString state;
	QString imagePath;
	QString methodName;
	QString methodPath;
	QString arg;
	QString cbCurIndex;
}PROCESSING;

namespace Ui {
class MainWindow;
}

void incThreadList(QString threadID);

class MainWindow : public QMainWindow
{
    Q_OBJECT
	QString libDirPath;
    QSqlDatabase db;
    QString dllPath;    

    QSqlTableModel* launchesModel;
    QSqlTableModel* processingModel;

    QLocalSocket *socket;
    QString currentFortune;
    quint16 blockSize;

	QMap<QString, QJsonObject> pluginList; //ключ -- путь к плагину;

	QTimer *timer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void incThreadList(QString threadID);

	QString pluginInfo(QString text);

//    static QMap<QString, int> threadList;
//    static void incThreadList(QString threadID);
public slots:
	void updateProgressBar();

private slots:
    void on_startProcessBtn_clicked();
    void on_browseBtn_clicked();
    void on_insertFileBtn_clicked();
    void on_browseDirBtn_clicked();
    void on_insertDirBtn_clicked();
	void addDLL(QString path);




//    void requestNewFortune();
    void readFortune();
    void displayError(QLocalSocket::LocalSocketError socketError);

    void on_pushButton_clicked();

	void on_methodCBox1_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
