#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T10:22:51
#
#-------------------------------------------------

QT       += widgets

TARGET = Inverse
TEMPLATE = lib

DEFINES += INVERSE_LIBRARY

SOURCES += inverse.cpp

HEADERS += inverse.h\
        inverse_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
