! include( ../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH +=\
                ../../libs/core \
                ../../libs/widgets \

DESTDIR = $$PLUGINS_DIR

LIBS += -L$$DESTDIR
LIBS += -L$$LIB_DIR
LIBS += -lRubusCore -lWidgets
