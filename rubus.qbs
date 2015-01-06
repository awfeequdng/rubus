import qbs.base 1.0

Project {
    name: "Rubus"

    property string buildPath: "../"
    property string buildPathLib: {
        if (qbs.targetOS.contains('windows'))
            return buildPath + "/bin";
        else
            return buildPath + "/lib";
    }
    property string buildPathBin:       buildPath + "/bin"
    property string buildPathPlugin:    buildPathBin + "/plugin"

    references : [ "src/libs/core/core.qbs" ]
}
