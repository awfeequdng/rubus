import qbs.base 1.0

DynamicLibrary {
    name: "Contractors"

    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: ["core", "gui", "network", "sql", "qml", "quick", "widgets"] }
    Depends { name: "RubusCore"}

    Qt.core.config : ["plugin"]
    cpp.includePaths : ["../../libs/core"]

    files : ["*.cpp", "*.h"]

    Group {
        name: "QML"
        files: ["qml/qmldir", "qml/*.qml"]
        qbs.install: true
        qbs.installDir: pluginsInstallDir + "/" + product.name
    }

    Group {
        fileTagsFilter: ["dynamiclibrary"]
        qbs.installDir: pluginsInstallDir + "/" + product.name
        qbs.install: true
    }

}
