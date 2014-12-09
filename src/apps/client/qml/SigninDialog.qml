import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import Rubus 1.0

Window {
    id: loginWindow
    property string username: edLogin.text;
    property string password: edPwd.text;
    property bool issave: ckSavePassword.checked;

    //signal onSignin;

    visible : true
    flags: Qt.Dialog
    modality: Qt.WindowModal
    width: 300
    height: 130
    title: "Rubus " + core.version

    MessageDialog {
        id : coreError
        title: "Core error"
        icon: StandardIcon.Critical
        standardButtons: StandardButton.Ok
    }

    Cryptor {
        id: cryptor
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        anchors.margins: 10
        rowSpacing: 6
        columnSpacing: 6

        Label { text: qsTr("Login:") }
        TextField {
            id: edLogin
            text : username
            placeholderText: qsTr("Enter login")
            Layout.fillWidth: true
            Keys.onEnterPressed : {
                if (text != "")
                    edPwd.focus = true
            }
            Keys.onReturnPressed : {
                if (text != "")
                    edPwd.focus = true
            }


            Keys.onEscapePressed : loginWindow.close();
        }

        Label { text: qsTr("Password:") }
        TextField {
            id: edPwd
            text: password
            placeholderText: qsTr("Enter password")
            echoMode: TextInput.Password
            Layout.fillWidth: true

            Keys.onEscapePressed : loginWindow.close();
            Keys.onEnterPressed : login()
            Keys.onReturnPressed : login()
        }

        Label { text: "" }
        CheckBox {
            id: ckSavePassword
            text: qsTr("Save password?")
            checked: issave
        }


        RowLayout {
            Layout.columnSpan: 2
            Item {
                Layout.fillWidth: true
            }

            Button {
                id: btnAccept
                text: qsTr("Sign in")
                Layout.preferredHeight: 30
                onClicked: {
                    if (edLogin.text == "") {
                        edLogin.focus = true
                    } else {
                        login();
                    }
                }
            }

            Button {
                id: btnCancel
                text: qsTr("Cancel")
                Layout.preferredHeight: 30
                onClicked: {
                    loginWindow.close();
                }
            }
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }    

    Component.onCompleted : {
        visible = true

        if (username == "") {
            edLogin.focus = true
        } else {
            edPwd.focus = true
        }

    }

    function login() {
        if (!core.login(username, password)) {
            coreError.text = core.errorString()
            coreError.visible = true
        } else {
            visible = false
            settings.setValue("lastUser", username)
            settings.setValue("isSave", issave)
            if (issave) {
                settings.setValue("pwd", cryptor.encode(password))
            }
        }
    }
}
