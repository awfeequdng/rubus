! include( ../../common.pri ) {
    error( Can not find the common.pri file! )
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Widgets
TEMPLATE = lib

DESTDIR = $$LIB_DIR

LIBS += -L$$LIB_DIR -lRubusCore

DEFINES += WIDGETS_LIBRARY

INCLUDEPATH += \
               ../core

HEADERS += \
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
    widgets/textformattoolbar.h \
    widgets/standardtabledialog.h \
    advitemmodel.h \
    widgets/advheaderview.h \
    widgets/advtableview.h \
    widgets/tabledialog.h \
    widgets/advschemetableview.h

SOURCES += \
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
    widgets/textformattoolbar.cpp \
    widgets/standardtabledialog.cpp \
    advitemmodel.cpp \
    widgets/advheaderview.cpp \
    widgets/advtableview.cpp \
    widgets/tabledialog.cpp \
    widgets/advschemetableview.cpp

FORMS += \
    widgets/standardtabledialog.ui
