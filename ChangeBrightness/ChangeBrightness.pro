#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T13:55:10
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++11

TARGET = ChangeBrightness
TEMPLATE = lib

DEFINES += CHANGEBRIGHTNESS_LIBRARY

SOURCES += ChangeBrightness.cpp

HEADERS += ChangeBrightness.h\
        changebrightness_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
