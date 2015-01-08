import qbs.base 1.0

Project {
    name: "Rubus"

    property string buildPath: "../"
    property string buildPathBin: buildPath + "/bin"
    property string buildPathPlugin: buildPathBin + "/plugins"
    property string buildPathLib: qbs.targetOS === 'windows' ? buildPath + "/bin" : buildPath + "/lib"

    references : [ "src/libs/core/core.qbs",
                   "src/apps/client/client.qbs",
                   "src/plugins/plugins.qbs"
    ]    
}
