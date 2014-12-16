import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import Rubus 1.0
import shared.qml 1.0

Window {
    id : reports
    width: 500
    height: 300
    title: qsTr("Reports");

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
                reports.title = qsTr("New report");
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
                reports.title = qsTr("Edit report #") + tabTable.item.currentId();
            }
        }
    }

    Action {
        id: acDelete
        text: qsTr("Delete")

        onTriggered: {
            tabTable.item.deleteSelections();
        }
    }

    Action {
        id: acPreview
        text: qsTr("Preview")

        onTriggered: {
            report.reportId = tabTable.item.currentId()
            report.show()
        }
    }


    SqlModel {
        id: reportModel
        query: "SELECT re_id, re_name, re_menu FROM reports"
        deleteQuery: "DELETE FROM reports WHERE re_id IN (:id)"
        primaryKeyRole: "re_id"
    }


    TabView {
        id: tabs
        tabsVisible: false
        anchors.fill: parent

        onCurrentIndexChanged: {
            if (currentIndex === 0)
                reports.title = qsTr("Reports");
        }

        Tab {
            id: tabTable
            active: true
            title: "Table"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 4

                function currentId() {
                    return reportModel.primaryKeyValue( table.currentRow )
                }

                function setCurrentId(id) {
                    for(var i = 0; i < table.rowCount; i++) {
                        if (reportModel.value(i, "re_id") === id) {
                            table.currentRow = i
                            table.selection.clear()
                            table.selection.select(i)
                        }
                    }
                }

                function deleteSelections() {
                    table.selection.forEach(
                                function(rowIndex) {
                                    reportModel.removeRow(rowIndex)
                                    console.log(rowIndex)
                                }  )
                    if (!reportModel.submit()) {
                        console.error(reportModel.errorString)
                    }
                }

                TableView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    selectionMode: SelectionMode.ExtendedSelection

                    id: table
                    focus: true
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

                RowLayout {
                    spacing: 4
                    anchors.margins: 4

                    Button { action: acAdd; Layout.minimumHeight: 30 }
                    Button { action: acEdit; Layout.minimumHeight: 30  }
                    Button { action: acDelete; Layout.minimumHeight: 30  }
                    Button { action: acPreview; Layout.minimumHeight: 30  }

                    Item { Layout.fillWidth: true}
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
                tabTable.item.setCurrentId(editTab.item.currentId())
                tabTable.focus = true

            }

            onCancel : {
                tabs.currentIndex = 0
            }
        }
    }

    onVisibleChanged: {
        tabs.currentIndex = 0
    }


    Component.onCompleted: {
        tabs.currentIndex = 0
    }

}
