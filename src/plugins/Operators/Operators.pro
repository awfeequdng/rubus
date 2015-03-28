! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Operators
TEMPLATE = lib

DEFINES += OPERATORS_LIBRARY

SOURCES += \
    operatorsplugin.cpp \
    models/operatormodel.cpp \
    widgets/operatortablewidget.cpp \
    widgets/operatoreditwidget.cpp

HEADERS +=\
    operatorsplugin.h \    
    operators_global.h \
    operatorsconstants.h \
    models/operatormodel.h \
    widgets/operatortablewidget.h \
    widgets/operatoreditwidget.h
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    operators.qrc

FORMS += \
    widgets/operatortablewidget.ui \
    widgets/operatoreditwidget.ui

