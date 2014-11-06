! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Base
TEMPLATE = lib

DEFINES += BASE_LIBRARY

SOURCES += base.cpp \
    models/itemmodel.cpp

HEADERS += base.h\
        base_global.h \
    constants.h \
    models/itemmodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
