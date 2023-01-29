/* import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
*/
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import io.qt.BackendStuff 1.0


Page {
        id: _root

        //property string bt_state_color : "grey"

        Button{
            id: _backButton
            x:0
            y:0
            text: " <-- Back"
            enabled: true
            onClicked: _stack.pop()
        }

        //child-object will be displayed inside root-window
        Text {
                id: _MoTalk
                text: "This is Motalk-App"
                x: 100
                y: 10
                //anchors.horizontalCenter: _root.horizontalCenter
                font.pointSize: 24; font.bold: true
        }

        Image {
                id: _backgroundFreq
                anchors.fill: parent
                width: parent.width
                height: parent.height
                source: "background_frequency.jpg"
                fillMode: Image.Tile; opacity: 0.3
        }

        BackendStuff {
            id: _afbBackend
    }

        //SingleKey{
        HexKeyboard{
            id: _singlekey
            x: 50
            y: 150

            onClick_1:  _textfield.text += _singlekey._1
            onClick_2:  _textfield.text += _singlekey._2
            onClick_3:  _textfield.text += _singlekey._3
            onClick_4:  _textfield.text += _singlekey._4
            onClick_5:  _textfield.text += _singlekey._5
            onClick_6:  _textfield.text += _singlekey._6
            onClick_7:  _textfield.text += _singlekey._7
            onClick_8:  _textfield.text += _singlekey._8
            onClick_9:  _textfield.text += _singlekey._9
            onClick_0:  _textfield.text += _singlekey._0

            onClick_e:  _textfield.text += _singlekey._e
            onClick_a:  _textfield.text += _singlekey._a
            onClick_d:  _textfield.text += _singlekey._d
            onClick_f:  _textfield.text += _singlekey._f
            onClick_c:  _textfield.text += _singlekey._c
            onClick_b:  _textfield.text += _singlekey._b
            onClick_Semicol: _textfield.text += _singlekey._Semicol

            //for deleting check SingleKey.qml
        }
                    Rectangle{
                        id: _textRect
                        Layout.fillWidth: true
                        x: 50
                        y: 400
                        Text{
                            text: _afbBackend.receiveBuffer
                            font.pointSize: 14
                            color: "black"
                        }
                    }

                    Rectangle {  //Pane is a rectangle whose color comes from the application's style, similar to Frame, without stroke around its border.
                        id: _rect
                        Layout.fillWidth: true
                        x: 100
                        y: 350

                        RowLayout {
                            width: parent.width

                            TextField {
                                id: _textfield
                                //y: 740
                                width: 120
                                height: 45
                                //topPadding: 8
                                font.pointSize: 18
                                //bottomPadding: 16
                                color: "black"

                                placeholderText: "Bth.-Addr.: aa:bb:cc:dd:ee:ff"
                                renderType: Text.QtRendering
                            }
                        }
                    }

                    Rectangle {  //Pane is a rectangle whose color comes from the application's style, similar to Frame, without stroke around its border.
                        id: _rect2
                        //Layout.fillWidth: true
                        x: 105
                        y: 410

                            Button {
                                id: _set_btadr
                                width: 310

                                Text{
                                    id: _btadr
                                    anchors.left: _set_btadr.left
                                    anchors.leftMargin: 47
                                    anchors.top: _set_btadr.top
                                    anchors.topMargin: 7
                                    //anchors.right: _set_btadr.right
                                    //anchors.rightMargin: 10
                                    text: "Set Bluetooth Send-Addr."
                                    font.pixelSize: 22
                                }

                                Image {
                                    id: _bt_symbol
                                    anchors.left: _set_btadr.left
                                    anchors.leftMargin: 10
                                    anchors.top: _set_btadr.top
                                    anchors.topMargin: 7
                                    anchors.bottom: _set_btadr.bottom
                                    anchors.bottomMargin: 7
                                    source: "qrc:/bluetooth_symbol.png"
                                    sourceSize.width: 45
                                    sourceSize.height: 40
                                }

                                enabled: true
                                onClicked:
                                {
                                    //_afbBackend.sendMessage("motalk2021","rewr",{"index":"blub"});
                                    _afbBackend.BluetoothAdr = _textfield.text;
                                }
                            }
                }


                    Button {
                        id: _roundbutton
                        text: "reload Bth-addr."
                        x: 420
                        y: 70
                        onClicked:
                        {
                            //_buttonColor.color = _afbBackend.BluetoothClr;
                            _textfield.text = _afbBackend.BluetoothAdr;
                        }

                        background: Rectangle {
                                color: "chartreuse"
                                border.width: 3
                                border.color: "blue"
                                radius: 25
                            }
                    }

                    Button{

                        checkable: true

                        id: _accConns
                        x: 80
                        y: 510
                        width: 370
                        height: 80

                        background:
                            Rectangle{
                                id: _buttonColor
                                objectName: "bt_rect"
                                color : _afbBackend.BluetoothClr
                                border.width: 5
                                border.color: "black"
                            }

                            Text{
                                anchors.left: _accConns.left
                                anchors.leftMargin: 50
                                anchors.top: _accConns.top
                                anchors.topMargin: 25
                                font.pixelSize: 24
                                text: "Bluetooth receive mode"
                            }
                    }

                    Rectangle{
                            id: _rectOn
                            color: "green"
                            y: 590
                            x: 80
                            width: 185
                            height: 40
                            border.width: 3
                            border.color: "black"





                            Text{
                                anchors.left: _rectOn.left
                                anchors.top: _rectOn.top
                                anchors.topMargin: 8
                                anchors.leftMargin: 80
                                text: "ON"
                                font.pixelSize: 20
                            }

                            MouseArea{
                                anchors.fill: parent

                                onClicked:
                                {
                                    _buttonColor.color = "green";
                                    _afbBackend.bt_recv_onoff(true);
                                }
                            }
                    }

                    Rectangle{
                            id: _rectOff
                            color: "red"
                            y: 590
                            x: 265
                            width: 185
                            height: 40
                            border.width: 3
                            border.color: "black"

                            Text{
                                anchors.left: _rectOff.left
                                anchors.top: _rectOff.top
                                anchors.topMargin: 8
                                anchors.leftMargin: 80
                                text: "OFF"
                                font.pixelSize: 20
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked:
                                {
                                    _buttonColor.color = "red";
                                    _afbBackend.bt_recv_onoff(false);
                                }
                            }
                    }
              }
