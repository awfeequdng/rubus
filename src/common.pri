ROOT_DIR = ../../bin
LIB_DIR  = ../../lib
win32: LIB_DIR  = $$ROOT_DIR
PLUGINS_DIR = $$ROOT_DIR/plugins

QT += sql xml widgets
CONFIG -= debug_and_release
INCLUDEPATH +=

DEFINES += QML_BASE_DIR=\\\"$$QML_BASE_DIR\\\"

OTHER_FILES += \


#[General]
#host=192.168.50.178
#database=rubus
#potr=5432
