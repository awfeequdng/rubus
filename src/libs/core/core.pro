! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

QT       += sql xml core gui

greaterThan(QT_MAJOR_VERSION, 4): {
    QT += printsupport
} else {
    include(qjson/qjson-backport.pri)
    INCLUDEPATH += qjson
}

TARGET = RubusCore
TEMPLATE = lib

DEFINES += CORE_LIBRARY

INCLUDEPATH += \
               $$NCREPORT_PATH/include \

DESTDIR = $$LIB_DIR

SOURCES += \
    core.cpp \
    pluginmanager.cpp \
    report.cpp \
    user.cpp \
    widgets/dlgauthorise.cpp \
    widgets/dlgauthsettings.cpp \
    widgets/mainwindow.cpp \
    iplugin.cpp

HEADERS +=\
    core.h \
    coreconstants.h \
    core_global.h \
    cryptor.h \
    pluginmanager.h \
    report.h \
    user.h \
    widgets/dlgauthorise.h \
    widgets/dlgauthsettings.h \
    widgets/mainwindow.h \
    version.h \
    iplugin.h

FORMS += \
    widgets/dlgauthorise.ui \
    widgets/dlgauthsettings.ui

RESOURCES +=

