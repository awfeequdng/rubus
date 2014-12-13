import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import Rubus 1.0
import shared.qml 1.0


Tab {
    id: reportEdit


    property int reportId : -1;

    signal cancel();
    signal saved();

    Report {
        id: report
        onSaved : {
            saved()
            console.log("Saved")
        }
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 10
        rowSpacing: 6
        columnSpacing: 6

        Text {
            text: qsTr("Name:")
        }

        TextField {
            id: edName
            Layout.fillWidth: true
            text: report.name
        }

        Text {
            text: qsTr("Menu:")
        }

        TextField {
            id: edMenu
            Layout.fillWidth: true
            text: report.menu
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
                    if (!report.save()) {
                        console.error(report.errorString)
                    }
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
