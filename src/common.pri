ROOT_DIR = ../../bin
LIB_DIR  = ../../lib
win32: LIB_DIR  = $$ROOT_DIR
PLUGINS_DIR = $$ROOT_DIR/plugins

QT += sql xml widgets
CONFIG -= debug_and_release
INCLUDEPATH +=
