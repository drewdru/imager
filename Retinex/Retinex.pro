#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T14:24:55
#
#-------------------------------------------------

QT       += widgets

TARGET = Retinex
TEMPLATE = lib

DEFINES += RETINEX_LIBRARY

SOURCES += Retinex.cpp \
    methode.cpp

HEADERS += Retinex.h\
        retinex_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
