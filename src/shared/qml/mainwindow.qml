import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import Rubus 1.0
import shared.qml 1.0

ApplicationWindow {
    property User user: core.currentUser();

    id: mainwindow
    width: settings.value("mainwindow/width", 500)
    height: settings.value("mainwindow/height", 200)
    title: "Rubus v" + core.version + " | " + user.name;
    SystemPalette {id: syspal}
    color: syspal.window

    Reports {
        id : reports_

    }

    Action {
        id: acReport
        text: qsTr("Reports")
        shortcut: Qt.Key_F4

        onTriggered: {
            //var rep = Qt.createQmlObject(core.loadQmlObject("Reports"), mainwindow, "c:\Projects\rubus\src\shared\qml");
            //rep.show();
            reports_.show()
        }
    }

    menuBar: MenuBar {
        Menu {
            title : "General"
            MenuItem {
                text : "Users"
                enabled: user.permission("Users")
            }

            MenuItem {
                action : acReport
            }

            MenuSeparator {}

            MenuItem {
                text : "Quit"
                shortcut: StandardKey.Quit
                onTriggered: mainwindow.close()

            }
        }
    }

    toolBar: ToolBar {
        ToolButton {
            action: acReport
        }

    }

//    Rectangle {
//        anchors.fill: parent
//        color : "blue"

//        Action {
//            id: acRepo
//            text: qsTr("Reports")
//            shortcut: Qt.Key_F4

//            onTriggered: console.log("<<<")
//        }
//    }






    statusBar: StatusBar {
        RowLayout {
            Label { text: "Host: " + core.host + ":" + core.port + "; database: " + core.database }
        }
    }

    onClosing: {
        settings.setValue("mainwindow/width", width)
        settings.setValue("mainwindow/height", height)
    }

    Component.onCompleted: {
        visible = true
    }
}
