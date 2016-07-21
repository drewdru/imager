TEMPLATE = app

QT += quick qml multimedia
QT += core gui widgets
CONFIG += c++11

qtHaveModule(quick) {
    QT += quick
}
INCLUDEPATH += ./lib
SOURCES += main.cpp \
    ImageProcessor.cpp \
    PluginInfo.cpp \
    PluginError.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    ImageProcessor.h \
    ProcessImagePlugins.h \
    PluginInfo.h \
    PluginError.h
