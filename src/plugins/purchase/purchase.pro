! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = purchase
TEMPLATE = lib

DEFINES += PURCHASE_LIBRARY

SOURCES += \
    purchaseplugin.cpp \
    views/requesttablewidget.cpp \
    models/requestmodel.cpp \
    views/requesteditdialog.cpp \
    controller.cpp \

HEADERS +=\
    purchaseplugin.h \    
    purchase_global.h \
    purchaseconstants.h \
    views/requesttablewidget.h \
    models/requestmodel.h \
    views/requesteditdialog.h \
    controller.h \
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    views/requesttablewidget.ui \
    views/requesteditdialog.ui \
    views/testedit.ui

