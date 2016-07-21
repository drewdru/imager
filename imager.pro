TEMPLATE = subdirs
TARGET = imager
SUBDIRS += \
#    camera-imager/camera-imager \
    colorCorrector \
    ChangeBrightness \
    Binarization \
    GrayWorld \
    Autolevels \
    Inverse \
    Retinex \
    Contrast \
    Hue \
    Gamma \
    HistogramEqualization \
    Gray \    


#INCLUDEPATH += ./lib
#INCLUDEPATH +=C:\\include
#LIBS += -LC:\\bin\\FTRSCANAPI.dll \
#C:\\bin\\bsdk6x.dll \
#C:\\bin\\bsdk6x.lib

#or

#LIBS += -L/usr/local/qwt-6.1.0/lib -lqwt
#QMAKE_LFLAGS += '-Wl,-rpath,/usr/local/qwt-6.1.0/lib' '-Wl,--rpath-link,/usr/local/qwt-6.1.0/lib'

#or

# Подключаем заголовочные файлы библиотеки
#INCLUDEPATH += include/MyLibrary
#CONFIG(debug, debug|release) {
#    # Подключаем debug-версии библиотек для разных платформ
#    win32: LIBS += -Llib -lMyLibraryd1
#    unix: LIBS += -Llib -L. -lMyLibraryd -Wl,-rpath,lib -Wl,-rpath,.
#} else {
#    # Подключаем release-версии библиотек для разных платформ
#    win32: LIBS += -Llib -lMyLibrary1
#    unix: LIBS += -Llib -L. -lMyLibrary -Wl,-rpath,lib -Wl,-rpath,.
#}

