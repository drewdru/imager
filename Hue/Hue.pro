#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T13:28:57
#
#-------------------------------------------------

QT       += widgets

TARGET = Hue
TEMPLATE = lib

DEFINES += HUE_LIBRARY

SOURCES += hue.cpp

HEADERS += hue.h\
        hue_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
