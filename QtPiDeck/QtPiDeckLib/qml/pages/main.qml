import QtQuick 2.12
import QtQuick.Window 2.12
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

        onTestingStatusChanged: {
            if (testingStatus === SettingsViewModel.TestingStatus.InProgress) {
                canvas.startAnimation()
            }
            else {
                canvas.stopAnimation(testingStatus)
            }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        GroupBox {
            title: qsTr("QtPiDeck server settings")
            label: GroupTitle {
                text: parent.title
                x: parent.leftPadding
            }

            ColumnLayout {
                Label {
                    text: qsTr("Ip")
                }

                TextField {
                    selectByMouse: true
                    text: settingsViewModel.deckServerAddress
                    onEditingFinished: settingsViewModel.deckServerAddress = text
                }

                Label {
                    text: qsTr("Port")
                }

                PortField {
                    text: settingsViewModel.deckServerPort
                    onEditingFinished: settingsViewModel.deckServerPort = text
                }

                RowLayout {
                    Button {
                        text: qsTr("Test connection")
                        onClicked: settingsViewModel.testConnection()
                        enabled: settingsViewModel.testEnabled
                    }

                    Canvas {
                        id: canvas

                        readonly property int animationDuration: 1200
                        readonly property int pauseDuration: 300
                        readonly property real initialPointA: -Math.PI / 2 - 0.1
                        readonly property real initialPointB: -Math.PI / 2 + 0.1

                        property real pointA: initialPointA
                        property real pointB: initialPointB
                        property bool running: false

                        onPointAChanged: requestPaint()
                        onPointBChanged: requestPaint()

                        function stopAnimation(testingStatus) {
                            pointAAnimation.stop()
                            pointBAnimation.stop()
                        }

                        function startAnimation() {
                            running = true
                            pointAAnimation.start()
                            pointBAnimation.start();
                        }

                        function onAnimationFinished() {
                            canvas.running = false
                        }

                        width: 40
                        height: 40
                        renderTarget: Canvas.Image
                        renderStrategy: Canvas.Cooperative

                        SequentialAnimation {
                            id: pointAAnimation
                            loops: Animation.Infinite
                            running: canvas.running
                            alwaysRunToEnd: true

                            onFinished: canvas.onAnimationFinished()

                            PauseAnimation {
                                duration: canvas.pauseDuration
                            }

                            NumberAnimation {
                                easing.type: Easing.InOutCubic
                                target: canvas
                                property: "pointA"
                                to: canvas.initialPointA + 2 * Math.PI
                                duration: canvas.animationDuration
                            }

                            NumberAnimation {
                                easing.type: Easing.InOutCubic
                                target: canvas
                                property: "pointA"
                                to: canvas.initialPointA
                                duration: 0
                            }
                        }

                        SequentialAnimation {
                            id: pointBAnimation
                            loops: Animation.Infinite
                            running: canvas.running
                            alwaysRunToEnd: true

                            NumberAnimation {
                                easing.type: Easing.InOutCubic
                                target: canvas
                                property: "pointB"
                                to: canvas.initialPointB + 2 * Math.PI
                                duration: canvas.animationDuration
                            }

                            PauseAnimation {
                                duration: canvas.pauseDuration
                            }

                            NumberAnimation {
                                easing.type: Easing.InOutCubic
                                target: canvas
                                property: "pointB"
                                to: canvas.initialPointB
                                duration: 0
                            }
                        }

                        onPaint: {
                            var ctx = getContext('2d')

                            let centerX = 20
                            let centerY = 20
                            let radius = 15

                            let circleColor = 'rgb(150, 150, 150)'

                            ctx.clearRect(0, 0, canvas.width, canvas.height);
                            ctx.resetTransform()

                            let fillColor =
                                settingsViewModel.testingStatus === SettingsViewModel.TestingStatus.Ok ? 'green' : 'red'

                            ctx.lineWidth = 4
                            ctx.fillStyle = fillColor
                            ctx.strokeStyle = circleColor
                            ctx.beginPath()
                            ctx.arc(centerX, centerY, radius, 0, Math.PI * 2)
                            if (settingsViewModel.testingStatus === SettingsViewModel.TestingStatus.Ok ||
                                    settingsViewModel.testingStatus === SettingsViewModel.TestingStatus.Timeout) {
                                ctx.fill()
                            }

                            ctx.stroke()

                            if (running &&
                                    settingsViewModel.testingStatus === SettingsViewModel.TestingStatus.InProgress) {
                                let sy = Math.sin(pointA) * radius + centerX
                                let sx = Math.cos(pointA) * radius + centerY
                                let ey = Math.sin(pointB) * radius + centerX
                                let ex = Math.cos(pointB) * radius + centerY

                                var gradient = ctx.createLinearGradient(sx, sy, ex, ey)
                                gradient.addColorStop(0, circleColor)
                                gradient.addColorStop(1, 'black')
                                ctx.strokeStyle = gradient

                                ctx.beginPath()
                                ctx.arc(centerX, centerY, radius, pointA, pointB, false)
                                ctx.lineCap = 'round';
                                ctx.stroke()
                            }
                        }
                    }
                }
            }
        }

        Button {
            text: qsTr("Save settings")
            onClicked: settingsViewModel.saveSettings();
        }
    }
}
