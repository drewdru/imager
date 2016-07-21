#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T18:40:54
#
#-------------------------------------------------

QT       += widgets

TARGET = Gray
TEMPLATE = lib

DEFINES += GRAY_LIBRARY

SOURCES += gray.cpp

HEADERS += gray.h\
        gray_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
