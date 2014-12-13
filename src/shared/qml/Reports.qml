import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
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
            tabs.currentIndex = 0
        }
    }

    Action {
        id: acEdit
        text: qsTr("Edit")

        onTriggered: {
            tabs.currentIndex = 0
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
            report.reportId = reportModel.primaryKeyValue( currentRow )
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
            title: "Table"

            TableView {
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
                    report.reportId = reportModel.primaryKeyValue( currentRow )
                    if (!report.load()) {
                        console.error(report.errorString)
                    } else {
                        tabs.currentIndex = 1
                    }
                }
            }
        }

        ReportEdit {
            id: tabEdit
            title: "Edit"

            onCancel: {
                tabs.currentIndex = 0
            }

            onSaved : {
                tabs.currentIndex = 0
                reportModel.refresh()
            }
        }
    }


    Component.onCompleted: {
        tabTable.active = true
    }
}
