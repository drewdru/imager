#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T02:36:07
#
#-------------------------------------------------

TARGET = Binarization
TEMPLATE = lib

DEFINES += BINARIZATION_LIBRARY

SOURCES += Binarization.cpp

HEADERS += Binarization.h\
        binarization_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
