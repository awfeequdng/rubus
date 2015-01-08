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
    ]

    Group {
        fileTagsFilter: product.type
        qbs.installDir: buildPathBin
        qbs.install: true
    }

    destinationDirectory: buildPathBin
}
