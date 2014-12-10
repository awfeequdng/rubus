ROOT_DIR = ../../bin
LIB_DIR  = ../../lib
win32: LIB_DIR  = $$ROOT_DIR
PLUGINS_DIR = $$ROOT_DIR/plugins

QT += sql xml widgets
CONFIG -= debug_and_release
INCLUDEPATH += \
              thirdparty/qaivlib


QML_BASE_DIR = $$PWD/shared/qml

DEFINES += QML_BASE_DIR=\\\"$$QML_BASE_DIR\\\"
