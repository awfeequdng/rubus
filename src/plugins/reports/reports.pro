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
}

INCLUDEPATH += $$NCREPORT_PATH/include

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

