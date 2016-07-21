#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T20:32:03
#
#-------------------------------------------------

QT       += widgets

TARGET = Contrast
TEMPLATE = lib

DEFINES += CONTRAST_LIBRARY

SOURCES += contrast.cpp

HEADERS += contrast.h\
        contrast_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
