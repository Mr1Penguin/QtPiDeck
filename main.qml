import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import data.connectionsettings 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    visibility: "FullScreen"
    title: qsTr("QtPiDeck")

    ConnectionSettings {
        id: connectionsettings
    }

    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: inputPanel.top

        Item {
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 15
            width: globalControls.implicitWidth

            ColumnLayout {
                id: globalControls
                RoundButton {
                    id:rbi
                    icon.name: "close"
                    onClicked: Qt.quit()
                }
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            id:mainLayout

            TextField {
                text: connectionsettings.hostAddress
                placeholderText: qsTr("Host address")

                onTextChanged: connectionsettings.hostAddress = text
            }

            TextField {
                text: connectionsettings.hostPort
                placeholderText: qsTr("Host port")

                onTextChanged: connectionsettings.hostPort = text
            }
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
