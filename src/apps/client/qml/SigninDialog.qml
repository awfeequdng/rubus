/***************************************************************************
 *   This file is part of the Rubus project                                *
 *   Copyright (C) 2012-2014 by Ivan Volkov                                *
 *   wulff007@gmail.com                                                    *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
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


    visible : true
    flags: Qt.Dialog
    modality: Qt.WindowModal
    width: 300
    height: lay.implicitHeight + 10
    title: "Rubus " + core.version

    SystemPalette {id: syspal}
    color: syspal.window

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
        id: lay
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
