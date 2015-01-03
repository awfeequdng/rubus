! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}
TEMPLATE = app

QT += qml quick core

SOURCES += main.cpp

RESOURCES += qml.qrc

DESTDIR = $$ROOT_DIR

INCLUDEPATH += ../../libs/core \
               ../../libs/core/widgets \
               ../../thirdparty/qaivlib \

LIBS += -L$$LIB_DIR -lRubusCore

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    qml/SigninDialog.qml \
    qml/MainWindow.qml \

