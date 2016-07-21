#-------------------------------------------------
#
# Project created by QtCreator 2015-12-20T16:20:25
#
#-------------------------------------------------

QT += multimedia core gui widgets network sql
CONFIG += c++11
QT += concurrent
qtHaveModule(quick) {
    QT += quick
}

TARGET = colorCorrector
TEMPLATE = app


#SOURCES += main.cpp\
#        mainwindow.cpp

#HEADERS  += mainwindow.h

#FORMS    += mainwindow.ui

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    processimageplugins.h \
    qdownloader.h \
    worker.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qdownloader.cpp \
    worker.cpp

