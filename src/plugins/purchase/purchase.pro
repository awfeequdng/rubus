! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = purchase
TEMPLATE = lib

DEFINES += PURCHASE_LIBRARY

SOURCES += \
    purchaseplugin.cpp \
    views/requesttablewidget.cpp \
    views/requesteditwidget.cpp \
    models/requestmodel.cpp

HEADERS +=\
    purchaseplugin.h \    
    purchase_global.h \
    purchaseconstants.h \
    views/requesttablewidget.h \
    views/requesteditwidget.h \
    models/requestmodel.h
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    views/requesttablewidget.ui \
    views/requesteditwidget.ui
