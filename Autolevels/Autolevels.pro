#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T23:02:52
#
#-------------------------------------------------

QT       += widgets

TARGET = Autolevels
TEMPLATE = lib

DEFINES += AUTOLEVELS_LIBRARY

SOURCES += Autolevels.cpp

HEADERS += Autolevels.h\
        autolevels_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
