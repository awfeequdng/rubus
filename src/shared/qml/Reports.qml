import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import Rubus 1.0

Window {
    width: 300
    height: 300

    ListModel {
       id: libraryModel
       ListElement{ title: "A Masterpiece" ; author: "Gabriel" }
       ListElement{ title: "Brilliance"    ; author: "Jens" }
       ListElement{ title: "Outstanding"   ; author: "Frederik"
       }
    }

    SqlModel {
        id: model
        query: "SELECT re_id, re_name FROM reports"
    }

    TableView {
        anchors.fill: parent
        TableViewColumn{ role: "re_id"  ; title: "Id" ; width: 100}
        TableViewColumn{ role: "re_name" ; title: "Name" ; width: 200 }
        model: model

        onClicked: {
            console.log("clicked")
        }
    }
}
