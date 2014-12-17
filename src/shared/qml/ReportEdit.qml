import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import Rubus 1.0
import shared.qml 1.0


Item {
    id: root
    signal cancel();
    signal save();

    Report {
        id:  report

        onSaved: root.save();

        onErrorStringChanged: {
            error.text = report.errorString
            error.open()
        }

        onLoaded: {
            edName.text = report.name
            edMenu.text = report.menu
            edFile.text = ""
        }
    }

    function currentId() {
        return report.reportId;
    }

    onCancel: {
        edName.text = ""
        edMenu.text = ""
        edFile.text = ""
    }

    MessageDialog {
        id : error
        title: "Error"
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Ok
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "CuteReport files (*.qtrp)", "All files (*)" ]
        onAccepted: {
            edFile.text = Qt.resolvedUrl(fileDialog.fileUrl)

        }
    }

    function load(id) {
        report.reportId = id
        if (!report.load()) {
            console.log(report.errorString)
            return false
        }

        return true
    }


    GridLayout {

        id: grid
        columns: 2
        anchors.fill: parent
        anchors.margins: 10
        rowSpacing: 6
        columnSpacing: 6

        Label {
            text: qsTr("Name:")
        }

        TextField {            
            id: edName
            Layout.fillWidth: true            
            Binding { target: report;  property: "name"; value: edName.text}
        }

        Label {
            text: qsTr("Menu:")
        }

        TextField {
            id: edMenu
            Layout.fillWidth: true
            Binding { target: report;  property: "menu"; value: edMenu.text}
        }

        Label {
            text: qsTr("From file:")
        }

        RowLayout {
            TextField {
                id: edFile
                Layout.fillWidth: true
                Binding { target: report;  property: "fromFile"; value: edFile.text }
            }

            Button {
                id:btnChooseFile
                text: "..."
                Layout.preferredWidth:  40

                onClicked: {
                    fileDialog.open()
                }
            }
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
        }


        RowLayout {
            Layout.columnSpan: 2
            Item {
                Layout.fillWidth: true
            }

            Button {
                id: btnAccept
                text: qsTr("Save")
                Layout.preferredHeight: 30
                onClicked: {
                    report.save()
                }
            }

            Button {
                id: btnCancel
                text: qsTr("Cancel")
                Layout.preferredHeight: 30
                onClicked: {
                    cancel()
                }
            }
        }
    }
}
