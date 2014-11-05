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

win32 {
    NCREPORT_PATH = C:/Qt/NCReport
    LIBS += -L$$NCREPORT_PATH/bin -lNCReport2
}
unix {
    NCREPORT_PATH = /home/wulff/NCReport2
    LIBS += -L$$NCREPORT_PATH/lib -lNCReport
    QMAKE_LFLAGS += -Wl,--rpath=$$NCREPORT_PATH/lib
}

INCLUDEPATH += \
               $$NCREPORT_PATH/include \

DESTDIR = $$LIB_DIR

SOURCES += \
    core.cpp \
    oooreportbuilder.cpp \
    pluginmanager.cpp \
    report.cpp \
    reportmanager.cpp \
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
    oooreportbuilder.h \
    pluginmanager.h \
    report.h \
    reportmanager.h \
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

