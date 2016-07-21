#include  <QQmlEngine>
#include  <QQmlComponent>
#include  <QtQuick/QQuickView>
#include <QApplication>
#include "ImageProcessor.h"
#include "ThreadProcessor.h"

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QGuiApplication>
#include <QStyleHints>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>


int main(int argc, char *argv[])
{
    qmlRegisterType<ImageProcessor>("ImageProcessor", 1, 0, "ImageProcessor");
    //qmlRegisterType<SqlQueryModel>("SqlQueryModel", 1, 0, "SqlQueryModel");

	QApplication app(argc, argv);

	QApplication::setApplicationDisplayName(QCoreApplication::translate("main",
																		"Imager"));
	QCommandLineParser parser;
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	parser.setApplicationDescription(QApplication::applicationDisplayName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	QQmlApplicationEngine engine;
	QQmlContext *context = engine.rootContext();

	QRect geometry = QApplication::primaryScreen()->availableGeometry();
	if (!QApplication::styleHints()->showIsFullScreen()) {
		const QSize size = geometry.size() * 4 / 5;
		const QSize offset = (geometry.size() - size) / 2;
		const QPoint pos = geometry.topLeft() + QPoint(offset.width(), offset.height());
		geometry = QRect(pos, size);
	}
	context->setContextProperty(QStringLiteral("initialX"), geometry.x());
	context->setContextProperty(QStringLiteral("initialY"), geometry.y());
	context->setContextProperty(QStringLiteral("initialWidth"), geometry.width());
	context->setContextProperty(QStringLiteral("initialHeight"), geometry.height());
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
