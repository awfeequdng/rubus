! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

win32 {
    NCREPORT_PATH = C:/Qt/NCReport
    CONFIG(debug, debug|release) {
        LIBS += -L$$NCREPORT_PATH/bin -lNCReportDebug2
    }else  {
        LIBS += -L$$NCREPORT_PATH/bin  -lNCReport2
    }
}
unix {
    NCREPORT_PATH = /home/wulff/NCReport2
    LIBS += -L$$NCREPORT_PATH/lib -lNCReport
    QMAKE_LFLAGS += -Wl,--rpath=$$NCREPORT_PATH/lib

    CUTEREPORT_PATH = /home/wulff/projects/cutereport
    CUTEREPORT_BUILD = /home/wulff/projects/build-CuteReport-Qt4_8_6_32/debug/build
    CUTEREPORT_BUILD_PLUGINS = $$CUTEREPORT_BUILD/cutereport/plugins
    LIBS += -L$$CUTEREPORT_BUILD
    LIBS += -lCuteReport -lCuteReportWidgets
    QMAKE_LFLAGS += -Wl,--rpath=$$CUTEREPORT_BUILD

}

DEFINES += CUTEREPORT_BUILD=\\\"$$CUTEREPORT_BUILD\\\"
DEFINES += CUTEREPORT_BUILD_PLUGINS=\\\"$$CUTEREPORT_BUILD_PLUGINS\\\"


INCLUDEPATH += $$NCREPORT_PATH/include \
               $$CUTEREPORT_PATH/src/core \
               $$CUTEREPORT_PATH/src/widgets/widgets \

TARGET = Reports
TEMPLATE = lib

DEFINES += REPORTS_LIBRARY

SOURCES += \
    reportsplugin.cpp \
    reportmanager.cpp \
    oooreportbuilder.cpp \
    widgets/reporteditwidget.cpp \
    widgets/reportsdialog.cpp \
    models/reportmodel.cpp \
    models/reporttypemodel.cpp

HEADERS +=\
    reportsplugin.h \    
    reports_global.h \
    reportsconstants.h \
    reportmanager.h \
    oooreportbuilder.h \
    widgets/reporteditwidget.h \
    widgets/reportsdialog.h \
    models/reportmodel.h \
    models/reporttypemodel.h
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    widgets/reporteditwidget.ui \
    widgets/reportsdialog.ui

