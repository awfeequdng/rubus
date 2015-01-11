import qbs.base 1.0

Application {
    name: "client"

    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: ["core", "gui", "network", "sql", "qml", "quick", "widgets"] }
    Depends { name: "RubusCore"}

    cpp.includePaths : ["../../libs/core"]

    files : [
        "client.qrc",
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h",
    ]

    Group {
        fileTagsFilter: product.type
        qbs.installDir: appInstallDir
        qbs.install: true
    }

    Group {
        name: "QML"
        prefix: ""
        files : ["qml/*.qml"]
    }

    cpp.rpaths : libRPaths
}
