#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T22:42:28
#
#-------------------------------------------------

QT       += widgets

TARGET = GrayWorld
TEMPLATE = lib

DEFINES += GRAYWORLD_LIBRARY

SOURCES += GrayWorld.cpp

HEADERS += GrayWorld.h\
        grayworld_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
