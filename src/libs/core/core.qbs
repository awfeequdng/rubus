import qbs.base 1.0

DynamicLibrary {
    name: "RubusCore"

    property string qmlBaseDir: ""
    property bool cutereport : true
    property string qutereportBuild : {
        if (qbs.targetOS.contains('windows'))
            return "C:/projects/cutereport/build-CuteReport-Desktop_Qt_5_4_0_MinGW_32bit/Debug/build";
        else
            return "/home/wulff/projects/cutereport/build-CuteReport-Desktop_Qt_5_4_0_GCC_32bit/Debug/build";
    }
    property string qutereportSrc :  {
        if (qbs.targetOS.contains('windows'))
            return "C:/projects/cutereport/cutereport";
        else
            return "/home/wulff/projects/cutereport/cutereport";
    }

    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: ["core", "gui", "network", "sql", "widgets"] }

    cpp.defines : {
        var defines = [
                    "CORE_LIBRARY",
                    "QML_BASE_DIR=\"" + qmlBaseDir + "\""
                ];

        if (cutereport) {
            defines.push("CUTEREPORT")
            defines.push("CUTEREPORT_BUILD=\"" + qutereportBuild + "\"")
            defines.push("CUTEREPORT_BUILD_PLUGINS=\"" + qutereportBuild + "/cutereport/plugins" + "\"")
        }

        return defines;
    }

    cpp.includePaths : {
        var includes = []

        if (cutereport) {
            includes.push(qutereportSrc + "/src/core")
            includes.push(qutereportSrc + "/src/widgets/widgets")
        }

        return includes;
    }

    cpp.dynamicLibraries: {
        var libs = []

        if (cutereport) {
            libs.push("CuteReport")
            libs.push("CuteReportWidgets")
        }

        return libs;
    }

    cpp.libraryPaths: {
        var paths = []

        if (cutereport) {
            paths.push(qutereportBuild)
        }

        return paths;
    }

    files : [
        "core_global.h",
        "cryptor.h",
        "qw.h",
        "version.h",
        "coreconstants.h",
        "core.cpp",
        "core.h",
        "pluginmanager.cpp",
        "pluginmanager.h",
        "report.cpp",
        "report.h",
        "user.cpp",
        "user.h",
        "iplugin.cpp",
        "iplugin.h",
        "settings.cpp",
        "settings.h",
        "sqlmodel.cpp",
        "sqlmodel.h",
        "sortfiltermodel.cpp",
        "sortfiltermodel.h",
    ]

    destinationDirectory: buildPathLib
}
