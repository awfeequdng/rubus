! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Reports
TEMPLATE = lib

DEFINES += REPORTS_LIBRARY

SOURCES += \
    reportsplugin.cpp \
    widgets/reporteditwidget.cpp \
    widgets/reportsdialog.cpp \
    models/reportmodel.cpp \
    models/reporttypemodel.cpp

HEADERS +=\
    reportsplugin.h \    
    reports_global.h \
    reportsconstants.h \
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

