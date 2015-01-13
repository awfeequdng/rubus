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
DEFINES += CUTEREPORT

win32 {
    CUTEREPORT_PATH = C:/Projects/cutereport/cutereport
    CUTEREPORT_BUILD = C:/projects/cutereport/build-CuteReport-Desktop_Qt_5_3_0_MinGW_32bit-Debug/build
}

unix {
    CUTEREPORT_PATH = /home/wulff/projects/cutereport
    CUTEREPORT_BUILD = /home/wulff/projects/build-CuteReport-Qt4_8_6_32/debug/build
    QMAKE_LFLAGS += -Wl,--rpath=$$CUTEREPORT_BUILD
}

CUTEREPORT_BUILD_PLUGINS = $$CUTEREPORT_BUILD/cutereport/plugins
LIBS += -L$$CUTEREPORT_BUILD
LIBS += -lCuteReport -lCuteReportWidgets

DEFINES += CUTEREPORT_BUILD=\\\"$$CUTEREPORT_BUILD\\\"
DEFINES += CUTEREPORT_BUILD_PLUGINS=\\\"$$CUTEREPORT_BUILD_PLUGINS\\\"

INCLUDEPATH += $$NCREPORT_PATH/include \
               $$CUTEREPORT_PATH/src/core \
               $$CUTEREPORT_PATH/src/widgets/widgets \

DESTDIR = $$LIB_DIR

SOURCES += \
    core.cpp \
    pluginmanager.cpp \
    report.cpp \
    user.cpp \
    iplugin.cpp \

HEADERS +=\
    core.h \
    coreconstants.h \
    core_global.h \
    cryptor.h \
    pluginmanager.h \
    report.h \
    user.h \
    version.h \
    iplugin.h \

RESOURCES += \
    core.qrc

