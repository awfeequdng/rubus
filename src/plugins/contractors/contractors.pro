! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

TEMPLATE = lib
TARGET = contractors
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Contractors

# Input
SOURCES += \
    contractors_plugin.cpp \
    contractor.cpp

HEADERS += \
    contractors_plugin.h \
    contractor.h

DISTFILES = qmldir \
    ContractorTable.qml

DESTDIR = $$PLUGINS_DIR/$$uri

qmlfiles.path = $$OUT_PWD/$$DESTDIR
qmlfiles.files = ContractorTable.qml  qmldir

INSTALLS+= qmlfiles

#QMAKE_POST_LINK += $$QMAKE_COPY $$replace($$list($$quote($$PWD/qmldir) $$DESTDIR), /, $$QMAKE_DIR_SEP)
#QMAKE_POST_LINK += $$QMAKE_COPY $$replace($$list($$quote($$PWD/ContractorTable.qml) $$DESTDIR), /, $$QMAKE_DIR_SEP)
#QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$copyfile) $$quote($$copydest) $$escape_expand(\\n\\t)