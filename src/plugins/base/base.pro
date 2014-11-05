! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = Base
TEMPLATE = lib

DEFINES += BASE_LIBRARY

SOURCES += base.cpp

HEADERS += base.h\
        base_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
