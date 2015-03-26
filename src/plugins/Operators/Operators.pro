! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Operators
TEMPLATE = lib

DEFINES += OPERATORS_LIBRARY

SOURCES += \
    operatorsplugin.cpp \
    widgets/operatortablewidget.cpp \
    models/operatormodel.cpp

HEADERS +=\
    operatorsplugin.h \    
    operators_global.h \
    operatorsconstants.h \
    widgets/operatortablewidget.h \
    models/operatormodel.h
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    operators.qrc

