! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}
TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

DESTDIR = $$ROOT_DIR

INCLUDEPATH += ../../libs/core \
               ../../libs/core/widgets \

LIBS += -L$$LIB_DIR -lRubusCore

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    qml/SigninDialog.qml
