! include( ../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH +=\
                ../../libs/core \
                ../../libs/widgets \
                ../../thirdparty/qaivlib \

DESTDIR = $$PLUGINS_DIR

LIBS += -L$$DESTDIR
LIBS += -L$$LIB_DIR
LIBS += -lRubusCore -lWidgets -lqaivlib
