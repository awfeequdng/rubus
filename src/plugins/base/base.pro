#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T16:11:06
#
#-------------------------------------------------

QT       -= core gui

TARGET = base
TEMPLATE = lib

DEFINES += BASE_LIBRARY

SOURCES += base.cpp

HEADERS += base.h\
        base_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
