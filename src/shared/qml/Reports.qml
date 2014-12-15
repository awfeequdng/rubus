import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import Rubus 1.0
import shared.qml 1.0

Window {
    id : reports
    width: 500
    height: 300

    SystemPalette {id: syspal}
    color: syspal.window

    Report {
        id : report
    }

    Action {
        id: acAdd
        text: qsTr("Add")

        onTriggered: {
            if ( !editTab.item.load( -1 ) ) {
                console.log("Error")
            } else {
                tabs.currentIndex = 1
            }
        }
    }

    Action {
        id: acEdit
        text: qsTr("Edit")

        onTriggered: {
            if (!editTab.item.load( tabTable.item.currentId() )) {
                console.log("Error")
            } else {
                tabs.currentIndex = 1
            }
        }
    }

    Action {
        id: acDelete
        text: qsTr("Delete")

        onTriggered: {
            tabs.currentIndex = 0
        }
    }

    Action {
        id: acPreview
        text: qsTr("Preview")

        onTriggered: {
            report.reportId =
            report.show()
        }
    }


    SqlModel {
        id: reportModel
        query: "SELECT re_id, re_name, re_menu FROM reports"
    }

    TabView {
        id: tabs
        tabsVisible: false
        anchors.fill: parent

        onCurrentIndexChanged: {
            switch(currentIndex) {
            case 0: reports.title = qsTr("Reports");
                break;
            case 1: reports.title = qsTr("Edit report");
                break;
            }
        }

        Tab {
            id: tabTable
            active: true
            title: "Table"


            TableView {
                function currentId() {
                   return reportModel.primaryKeyValue( table.currentRow )
                }

                id: table
                focus: true
                anchors.fill: parent
                TableViewColumn {
                    role: "re_id"
                    title: qsTr("Id")
                    width: 50
                }
                TableViewColumn {
                    role: "re_name"
                    title: qsTr("Name")
                    width: 200
                }
                TableViewColumn {
                    role: "re_menu"
                    title: qsTr("Menu")
                    width: 200
                }

                model: reportModel

                onDoubleClicked: {
                    acEdit.trigger()
                }
            }
        }


        Tab {
            id: editTab
            active: true
            source: "ReportEdit.qml"
        }

        Connections {
            target: editTab.item

            onSave : {
                tabs.currentIndex = 0
                reportModel.refresh()
            }

            onCancel : {
                tabs.currentIndex = 0
            }
        }
    }


    Component.onCompleted: {
        tabs.currentIndex = 0
    }
}
