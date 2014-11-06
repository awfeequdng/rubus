! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Widgets
TEMPLATE = lib

DESTDIR = $$LIB_DIR

DEFINES += WIDGETS_LIBRARY

INCLUDEPATH += \
               ../core

HEADERS += \
    itemmodel.h \
    widgets_global.h \
    widgets/advcombobox.h \
    widgets/advdateedit.h \
    widgets/advlineedit.h \
    widgets/advspinbox.h \
    widgets/doublespinbox.h \
    widgets/doublespinboxdelegate.h \
    widgets/editdialog.h \
    widgets/editwidgetinterface.h \
    widgets/modelselectoredit.h \
    widgets/printbutton.h \
    widgets/printcombobox.h \
    widgets/tableeditdialog.h \
    widgets/tableeditinterface.h \
    widgets/textformattoolbar.h

SOURCES += \
    itemmodel.cpp \
    widgets/advcombobox.cpp \
    widgets/advdateedit.cpp \
    widgets/advlineedit.cpp \
    widgets/advspinbox.cpp \
    widgets/doublespinbox.cpp \
    widgets/doublespinboxdelegate.cpp \
    widgets/editdialog.cpp \
    widgets/editwidgetinterface.cpp \
    widgets/modelselectoredit.cpp \
    widgets/printbutton.cpp \
    widgets/printcombobox.cpp \
    widgets/tableeditdialog.cpp \
    widgets/tableeditinterface.cpp \
    widgets/textformattoolbar.cpp
