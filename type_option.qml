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

        /*
         // WINDOW PROPERTIES
        visible: true
        width: 600
        height: 680
        color:"steelblue"
    */

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

        SingleKey{
            id: _singlekey
            x: 50
            y: 100

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

            onClick_q:  _textfield.text += _singlekey._q
            onClick_w:  _textfield.text += _singlekey._w
            onClick_e:  _textfield.text += _singlekey._e
            onClick_r:  _textfield.text += _singlekey._r
            onClick_t:  _textfield.text += _singlekey._t
            onClick_z:  _textfield.text += _singlekey._z
            onClick_u:  _textfield.text += _singlekey._u
            onClick_i:  _textfield.text += _singlekey._i
            onClick_o:  _textfield.text += _singlekey._o
            onClick_p:  _textfield.text += _singlekey._p
            onClick_a:  _textfield.text += _singlekey._a
            onClick_s:  _textfield.text += _singlekey._s
            onClick_d:  _textfield.text += _singlekey._d
            onClick_f:  _textfield.text += _singlekey._f
            onClick_g:  _textfield.text += _singlekey._g
            onClick_h:  _textfield.text += _singlekey._h
            onClick_j:  _textfield.text += _singlekey._j
            onClick_k:  _textfield.text += _singlekey._k
            onClick_l:  _textfield.text += _singlekey._l
            onClick_y:  _textfield.text += _singlekey._y
            onClick_x:  _textfield.text += _singlekey._x
            onClick_c:  _textfield.text += _singlekey._c
            onClick_v:  _textfield.text += _singlekey._v
            onClick_b:  _textfield.text += _singlekey._b
            onClick_n:  _textfield.text += _singlekey._n
            onClick_m:  _textfield.text += _singlekey._m

            onClick_Semicol: _textfield.text += _singlekey._Semicol
            onClick_Period: _textfield.text += _singlekey._Period
            onClick_Questm: _textfield.text += _singlekey._Questm


            //for deleting check SingleKey.qml
        }

                    Rectangle{
                        id: _textRect
                        Layout.fillWidth: true
                        width: 700
                        x: 50
                        y: 350
                        Text{
                            text: _afbBackend.receiveBuffer
                            font.pointSize: 14
                            color: "black"
                        }
                    }

                    Rectangle {  //Pane is a rectangle whose color comes from the application's style, similar to Frame, without stroke around its border.
                        id: _rect
                        //Layout.fillWidth: true
                        x: 50
                        y: 450
                        width: 500

                        RowLayout {
                            width: parent.width

                            TextField {
                                id: _textfield
                                y: 440
                                Layout.fillWidth: true
                                height: 45
                                //topPadding: 8
                                font.pointSize: 14
                                //bottomPadding: 16

                                placeholderText: "User Input"
                                renderType: Text.QtRendering
                            }
                        }
                    }

                    Rectangle {  //Pane is a rectangle whose color comes from the application's style, similar to Frame, without stroke around its border.
                        id: _rect2
                        Layout.fillWidth: true
                        x: 50
                        y: 500
                        RowLayout {
                            width: parent.width
                            Button {
                                id: _sendButton

                                Text{
                                    id: _text_send
                                    anchors.left: _sendButton.left
                                    anchors.leftMargin: 45
                                    anchors.top: _sendButton.top
                                    anchors.topMargin: 10
                                    text: "Send"
                                    font.pixelSize: 18
                                }

                                Image {
                                    id: _bt_symbol
                                    anchors.left: _sendButton.left
                                    anchors.leftMargin: 10
                                    anchors.top: _sendButton.top
                                    anchors.topMargin: 7
                                    anchors.bottom: _sendButton.bottom
                                    anchors.bottomMargin: 7
                                    source: "qrc:/bluetooth_symbol.png"
                                    sourceSize.width: 45
                                    sourceSize.height: 40
                                }

                                enabled: true
                                onClicked:
                                {
                                    //_afbBackend.sendMessage("motalk2021","ping", true);
                                    //_afbBackend.sendMessage("motalk2021","rewr",{"index":"blub"});
                                    //_afbBackend.sendMessage("motalk2021","rewr",true);
                                    _afbBackend.UserInput = _textfield.text;
                                    _afbBackend.bth_usage();
                                }
                            }

                            Button {
                                id: _connectButton
                                text: "connect AFB-ws"
                                enabled: true
                                onClicked:
                                {
                                    _afbBackend.connectClicked();
                                    _afbBackend.sendClicked("GET /api?token=123456 HTTP/1.1\r\nHost: 127.0.0.1:1234\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: MfpIVG082jFTV7SxTNNijQ==\r\nSec-WebSocket-Protocol: x-afb-ws-json1\r\nContent-Length: 0\r\n\r\n");
                                }
                            }

                            Button {
                                id: _disconnectButton
                                text: "disconnect AFB-ws"
                                enabled: true
                                onClicked:
                                {
                                    _afbBackend.disconnectClicked();
                                }

                            }

                            Button {
                                id: _UserInput
                                //text: "ParseUserInput"
                                text: "WebSocket Send"
                                enabled: true
                                onClicked:
                                {
                                    _afbBackend.UserInput = _textfield.text;
                                    _afbBackend.sendMessage("motalk2021","rewr");
                                    //_afbBackend.sendKeyboardInput("testbutton");
                                    //_afbBackend.receiveKeypress = _textfield.selectAll();
                                }
                            }
                        }
                }

                    TextField {
                        id: _rcvtext
                        x: 50
                        y: 580
                        Layout.fillWidth: true
                        //width: 1700
                        //height: 45
                        //topPadding: 8
                        font.pointSize: 18
                        //bottomPadding: 16
                        color: "blue"

                        placeholderText: "Received textmessage..."
                        renderType: Text.QtRendering
                    }

                    Button {
                        id: _textmsg_notification
                        visible: _afbBackend.Bth_txtmsg_recv   // true or false
                        anchors.right: _rcvtext.right
                        anchors.top: _rcvtext.top
                        anchors.rightMargin: 0
                        anchors.topMargin: -20

                        background: Rectangle {
                                width: (parent.width<parent.height?parent.width:parent.height)*2
                                height: width
                                color: "red"
                                border.width: 1
                                border.color: "red"
                                radius: width*0.5
                            }
                    }

                    Button {
                        id: _roundbutton_txtmsg
                        text: "reload message"
                        x: 420
                        y: 580
                        onClicked:
                        {
                            //_rcvtext.text = "hallo1234";
                            _rcvtext.text = _afbBackend.print_msg_recv;
                            _textmsg_notification.visible = false;
                        }

                        background: Rectangle {
                                color: "chartreuse"
                                border.width: 3
                                border.color: "blue"
                                radius: 25
                            }
                    }
         }

