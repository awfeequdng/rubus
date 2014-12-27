! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

QT       += sql xml core

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
    CUTEREPORT_BUILD = C:/Projects/cutereport/build-CuteReport-Desktop_Qt_5_3_0_MinGW_32bit-Debug/build
}

unix {
    CUTEREPORT_PATH = /home/wulff/projects/cutereport/cutereport
    CUTEREPORT_BUILD = /home/wulff/projects/cutereport/build-CuteReport-Desktop_Qt_5_4_0_GCC_64bit/Debug/build
    QMAKE_LFLAGS += -Wl,--rpath=$$CUTEREPORT_BUILD
}

CUTEREPORT_BUILD_PLUGINS = $$CUTEREPORT_BUILD/cutereport/plugins
LIBS += -L$$CUTEREPORT_BUILD
LIBS += -lCuteReport -lCuteReportWidgets

message($$LIBS)

DEFINES += CUTEREPORT_BUILD=\\\"$$CUTEREPORT_BUILD\\\"
DEFINES += CUTEREPORT_BUILD_PLUGINS=\\\"$$CUTEREPORT_BUILD_PLUGINS\\\"

INCLUDEPATH += $$CUTEREPORT_PATH/src/core \
               $$CUTEREPORT_PATH/src/widgets/widgets \

DESTDIR = $$LIB_DIR

SOURCES += \
    core.cpp \
    pluginmanager.cpp \
    report.cpp \
    user.cpp \
    iplugin.cpp \
    settings.cpp \
    sqlmodel.cpp

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
    settings.h \
    sqlmodel.h

RESOURCES +=

