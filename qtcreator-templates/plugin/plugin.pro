! include( ../plugins.pri ) {
    error( Can not find the plugins.pri file! )
}

TARGET = %ProjectName%
TEMPLATE = lib

DEFINES += %ProjectName:u%_LIBRARY

SOURCES += \
    %PluginName:l%.%CppSourceSuffix% \

HEADERS +=\
    %PluginName:l%.%CppHeaderSuffix% \    
    %ProjectName:l%_global.%CppHeaderSuffix% \
    %ProjectName:l%constants.%CppHeaderSuffix% \
    

unix {
    target.path = /usr/lib
    INSTALLS += target
}
