! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

QT       -= sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Widgets
TEMPLATE = lib

DESTDIR = $$LIB_DIR

DEFINES += WIDGETS_LIBRARY

INCLUDEPATH += \
