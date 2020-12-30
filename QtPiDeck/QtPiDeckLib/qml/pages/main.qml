import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtPiDeck 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    //visibility: "FullScreen"
    title: qsTr("QtPiDeck")

    SettingsViewModel {
        id: settingsViewModel
    }

    ColumnLayout {
        TextField {
            id: addressField
            selectByMouse: true
            text: settingsViewModel.deckServerAddress
            onEditingFinished: settingsViewModel.deckServerAddress = text
        }

        PortField {
            text: settingsViewModel.deckServerPort
            onEditingFinished: settingsViewModel.deckServerPort = port
        }

        Button {
            text: "Press me!"
            onClicked: settingsViewModel.saveSettings();
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
