import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

//import QtPiDeck 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    //visibility: "FullScreen"
    title: qsTr("QtPiDeck")

    /*Component.onCompleted: {
        DataManager.loadConfiguration()
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
            anchors.rightMargin: 15 + dropDownButton.width
            width: globalControls.implicitWidth

            Rectangle {
                id: globalControls

                RoundButton {
                    id: dropDownButton
                    icon.name: "dropdown"
                    icon.width: 32
                    icon.height: 32
                    z: 1
                    onClicked: {
                        icon.name = icon.name == "dropdown" ? "dropup" : "dropdown"
                    }
                    Component.onCompleted: {
                        console.log(width)
                    }
                }

                ListModel{
                    id: globalOptions
                    ListElement {
                        iconName: "close"
                    }
                }

                signal clickIndexSignal(var i)
                onClickIndexSignal: {
                    switch(i) {
                    case 0:
                        Qt.quit();
                    }
                }

                Repeater {
                    model: globalOptions
                    RoundButton {
                        icon.name: iconName
                        icon.width: dropDownButton.icon.width
                        icon.height: dropDownButton.icon.height
                        onClicked: globalControls.clickIndexSignal(index)
                        y: 0
                        id: globalOption

                        states: State {
                            name: "visible"
                            when: dropDownButton.icon.name == "dropup"
                            PropertyChanges {
                                target: globalOption
                                y: (globalOption.height + 8) * (index + 1)
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
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            id:mainLayout

            TextField {
                text: DataManager.appConfiguration.hostAddress
                placeholderText: qsTr("Host address")

                onTextChanged: DataManager.appConfiguration.hostAddress = text
            }

            TextField {
                text: DataManager.appConfiguration.hostPort
                placeholderText: qsTr("Host port")

                onTextChanged: DataManager.appConfiguration.hostPort = text
            }
        }
    }*/

    TextField {}

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
