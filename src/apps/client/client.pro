! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

DESTDIR = $$ROOT_DIR

INCLUDEPATH += ../../libs/core \
               ../../libs/core/widgets \


SOURCES += main.cpp



LIBS += -L$$LIB_DIR -lRubusCore

RC_FILE += client.rc
