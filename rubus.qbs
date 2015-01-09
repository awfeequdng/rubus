import qbs.base 1.0

Project {
    name: "Rubus"

    property string libDirName: "lib"
    property string appInstallDir: "bin"
    property string libInstallDir: qbs.targetOS.contains("windows") ? "bin" : libDirName
    property string pluginsInstallDir: appInstallDir + "/plugins"
    property bool enableRPath: true
    property stringList libRPaths: {
        if (!project.enableRPath)
            return undefined;
        if (qbs.targetOS.contains("linux"))
            return ["$ORIGIN/../" + libDirName];
        if (qbs.targetOS.contains("osx"))
            return ["@loader_path/../" + libDirName]
    }

    references : [ "src/libs/core/core.qbs",
                   "src/apps/client/client.qbs",
                   "src/plugins/plugins.qbs"
    ]    


}
