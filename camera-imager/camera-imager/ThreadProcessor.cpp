//#include "ThreadProcessor.h"

//#include <QDebug>
//#include <QtSql>

//#include  <QQmlEngine>
//#include  <QQmlComponent>
//#include  <QtQuick/QQuickView>
//#include <QApplication>
//#include <QQmlApplicationEngine>
//#include <QtQml/QQmlContext>

//ThreadProcessor::ThreadProcessor(QObject *parent)
//    : QObject(parent)
//{
//    db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("my_db.sqlite");
//    if (!db.open()) {
//        qDebug() << "БД не открыта!";
//    }

//    QSqlQuery a_query;
//    // DDL query
//    QString str = "CREATE IF NOT EXISTS TABLE test1(a INT, b TEXT);";
//    if (!a_query.exec(str)) {
//        qDebug() << "";
//    }

//    // DML
//    QString str_insert = "INSERT INTO test1(a, b) "
//                         "VALUES (%1, '%2');";
//    str = str_insert.arg("14")
//            .arg("hello world str.");
//    if (!a_query.exec(str)) {
//        qDebug() << "данные не вставляются";
//    }

//    QSqlQuery q;
//    q.exec("SELECT * FROM test1");
//    while(q.next()){
//        qDebug()<<QString::number(q.value("a").toInt());
//        qDebug()<<q.value("b").toString();
//    }
//    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

//    //    QQuickView view;
//    //    QQmlContext *ctxt = view.rootContext();
//    //    ctxt->setContextProperty("myModel", QVariant::fromValue(dataList));
//    //    view.setSource(QUrl("qrc:view.qml"));
//    //    view.show();
//}
