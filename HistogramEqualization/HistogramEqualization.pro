#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T17:20:25
#
#-------------------------------------------------

QT       += widgets

TARGET = HistogramEqualization
TEMPLATE = lib

DEFINES += HISTOGRAMEQUALIZATION_LIBRARY

SOURCES += histogramequalization.cpp

HEADERS += histogramequalization.h\
        histogramequalization_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
