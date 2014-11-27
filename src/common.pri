ROOT_DIR = ../../bin
LIB_DIR  = ../../lib
win32: LIB_DIR  = $$ROOT_DIR
PLUGINS_DIR = $$ROOT_DIR/plugins

QT += sql xml
CONFIG -= debug_and_release

greaterThan(QT_MAJOR_VERSION, 4) :  QT += widgets

INCLUDEPATH +=
