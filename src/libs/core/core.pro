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
    CONFIG(debug, debug|release) {
        LIBS += -L$$NCREPORT_PATH/bin -lNCReportDebug2
    }else  {
        LIBS += -L$$NCREPORT_PATH/bin  -lNCReport2
    }
    CUTEREPORT_PATH = C:/Projects/cutereport/cutereport
    CUTEREPORT_BUILD = C:/Projects/cutereport/build-CuteReport-Qt4_8_5_mingw/Debug/build
}

unix {
    NCREPORT_PATH = /home/wulff/NCReport2
    LIBS += -L$$NCREPORT_PATH/lib -lNCReport
    QMAKE_LFLAGS += -Wl,--rpath=$$NCREPORT_PATH/lib

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
    widgets/dlgauthorise.cpp \
    widgets/dlgauthsettings.cpp \
    widgets/mainwindow.cpp \
    iplugin.cpp \
    oooreportbuilder.cpp \
    reportmanager.cpp

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
    iplugin.h \
    oooreportbuilder.h \
    reportmanager.h

FORMS += \
    widgets/dlgauthorise.ui \
    widgets/dlgauthsettings.ui

RESOURCES +=

