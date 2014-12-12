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



    SqlModel {
        id: model
        query: "SELECT re_id, re_name, re_type FROM reports"
    }

    Component {
        id: typeDelegate
        Text {
            anchors.verticalCenter: parent.verticalCenter
            color: styleData.textColor
            elide: styleData.elideMode
            //text: Report.CuteReportEngine
            renderType: Text.NativeRendering
            style: Text.Normal
        }
    }

    TableView {
        focus: true
        anchors.fill: parent
        TableViewColumn {
            role: "re_id"
            title: "Id"
            width: 100
        }
        TableViewColumn {
            role: "re_name"
            title: "Name"
            width: 200
        }
        TableViewColumn {
            role: "re_type"
            title: "Type"
            width: 200
            delegate:typeDelegate
        }

        Report {
            id: report
        }


        model: model

        ReportEdit {
            id : repEdit
        }

        onDoubleClicked: {
//            var reportEdit = Qt.createQmlObject(core.loadQmlObject("ReportEdit"), reports, "");
//            reportEdit.show();
            repEdit.show()
//            console.log(model.primaryKeyValue(currentRow))
//            report.reportId = model.primaryKeyValue(currentRow)
//            report.show()
        }
    }
}
