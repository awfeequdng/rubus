import qbs.base 1.0

DynamicLibrary {
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: ["core", "network", "sql", "qml", "quick"] }
    Depends { name: "RubusCore"}

    Qt.core.config : ["plugin"]
    cpp.includePaths : ["../../libs/core"]

    property string uri: pluginsInstallDir + "/" + name;

    Group {
        name: "Sources"
        files : ["*.cpp", "*.h"]
    }

    Group {
        name: "Qml"
        files: ["qmldir", "*.qml"]
        qbs.install: true
        qbs.installDir: uri
    }

    Group {
        fileTagsFilter: ["dynamiclibrary"]
        qbs.installDir: uri
        qbs.install: true
    }
}
