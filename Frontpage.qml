import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import io.qt.BackendStuff 1.0

Page {
        id: _front
        header: Label{
            color: "steelblue"
            padding: 10
            text: "This is MoTalk-App"
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        BackendStuff{
            id: _afbBackend

            /*
            onBth_msg_recvChanged:
            {
                //_msg_notification.visible = true;
                _rect2.color = "yellow"
            }*/
        }

            Image {
                    id: _backgroundFreq
                    width: parent.width
                    height: parent.height
                    source: "background_frequency.jpg"
                    fillMode: Image.Tile; opacity: 0.3
            }

            ListView{
                    id: _listView
                    anchors.fill: parent
                    topMargin: 80
                    leftMargin: 60
                    bottomMargin: 80
                    rightMargin: 60
                    spacing: 40

                    //model: model
                    Rectangle{
                            id: _rect1
                            color: "grey"
                            y: 100
                            x: 50
                            width: _listView.width - _listView.leftMargin - _listView.rightMargin
                            height: _avatar_keyboard.implicitHeight
                            border.width: 5
                            border.color: "black"

                            Image {
                                id: _avatar_keyboard
                                sourceSize.width: 200
                                sourceSize.height: 160
                                anchors.left: _rect1.left
                                anchors.leftMargin: 10
                                anchors.top: _rect1.top
                                anchors.topMargin: 7
                                anchors.bottom: _rect1.bottom
                                anchors.bottomMargin: 7
                                source: "qrc:/keyboard_pc.png"
                            }

                            Text{
                                anchors.right: _rect1.right
                                anchors.top: _rect1.top
                                anchors.topMargin: 35
                                anchors.rightMargin: 30
                                text: "Choose type-option"
                                font.pixelSize: 26
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: _front.StackView.view.push("qrc:/type_option.qml")   //this is a URL
                                    //items that StackView creates from Components or URLs are destroyed by the StackView.pop()
                            }
                    }


                    Rectangle{
                            id: _rect2
                            color: "grey"
                            y: 300
                            x: 50
                            width: _listView.width - _listView.leftMargin - _listView.rightMargin
                            height: _avatar_voice.implicitHeight
                            border.width: 5
                            border.color: "black"

                            Image {
                                id: _avatar_voice
                                anchors.left: _rect2.left
                                anchors.leftMargin: 40
                                anchors.top: _rect2.top
                                anchors.topMargin: 7
                                anchors.bottom: _rect2.bottom
                                anchors.bottomMargin: 7
                                source: "qrc:/loudspeaker_boy.png"
                                sourceSize.width: 110
                                sourceSize.height: 100
                            }

                            Text{
                                anchors.right: _rect2.right
                                anchors.top: _rect2.top
                                anchors.topMargin: 35
                                anchors.rightMargin: 40
                                text: "Choose speech-option"
                                font.pixelSize: 28
                            }

                            /*
                            Button {
                                id: _msg_notification
                                visible: _afbBackend.Bth_msg_recv
                                anchors.right: _rect2.right
                                anchors.top: _rect2.top
                                anchors.rightMargin: 10
                                anchors.topMargin: -20

                                background: Rectangle {
                                        width: (parent.width<parent.height?parent.width:parent.height)*3
                                        height: width
                                        color: "red"
                                        border.width: 1
                                        border.color: "red"
                                        radius: width*0.5
                                    }
                            } */

                            MouseArea{
                                anchors.fill: parent
                                onClicked: _front.StackView.view.push("qrc:/speech_option.qml") //this is a URL
                                //items that StackView creates from Components or URLs are destroyed by the StackView
                            }
                    }

                    //für Bluetooth-usage
                    //BackendStuff {
                      //  id: _afbBackend
                    //}

                    Rectangle{
                        id: _rect3
                        color: "grey"
                        y: 500
                        x: 50
                        width: _listView.width - _listView.leftMargin - _listView.rightMargin
                        height: _avatar_voice.implicitHeight
                        border.width: 5
                        border.color: "black"

                        Image {
                            id: _bt_symbol
                            anchors.left: _rect3.left
                            anchors.leftMargin: 50
                            anchors.top: _rect3.top
                            anchors.topMargin: 7
                            anchors.bottom: _rect3.bottom
                            anchors.bottomMargin: 7
                            source: "qrc:/bluetooth_symbol.png"
                            sourceSize.width: 110
                            sourceSize.height: 100
                        }

                        Text{
                            anchors.right: _rect3.right
                            anchors.top: _rect3.top
                            anchors.topMargin: 35
                            anchors.rightMargin: 75
                            text: "Bluetooth Settings"
                            font.pixelSize: 28
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked:
                            {
                                // _afbBackend.bluetooth_test();
                                _front.StackView.view.push("qrc:/bluetooth_option.qml") //this is a URL
                                //items that StackView creates from Components or URLs are destroyed by the StackView
                            }
                        }

                    }
                }
    }
