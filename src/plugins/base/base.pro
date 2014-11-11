! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Base
TEMPLATE = lib

DEFINES += BASE_LIBRARY

SOURCES += \
    models/itemmodel.cpp \
    widgets/itemeditwidget.cpp \
    baseplugin.cpp \
    item.cpp \
    models/itemtypemodel.cpp \
    widgets/contractortabledialog.cpp \
    models/contractormodel.cpp

HEADERS +=\
        base_global.h \
    constants.h \
    models/itemmodel.h \
    widgets/itemeditwidget.h \
    baseplugin.h \
    item.h \
    models/itemtypemodel.h \
    widgets/contractortabledialog.h \
    models/contractormodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    widgets/itemeditwidget.ui \
    widgets/contractortabledialog.ui
