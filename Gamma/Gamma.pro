#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T14:46:33
#
#-------------------------------------------------

QT       += widgets

TARGET = Gamma
TEMPLATE = lib

DEFINES += GAMMA_LIBRARY

SOURCES += gamma.cpp

HEADERS += gamma.h\
        gamma_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
