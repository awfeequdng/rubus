import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import Rubus 1.0

Window {
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




        model: model

//        highlightOnFocus: Rectangle {
//            color: "lightblue"
//            width: parent.width
//        }






        //selection

        onDoubleClicked: {
            console.log(model.primaryKeyValue(currentRow))
        }
    }
}
