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
        //signal triggert
        property int voice_cap_time: 40
        property string textclr: "grey"

        BackendStuff{
            id: _afbBackend

            onSpeechFinished: {
                _speech.color = "grey";
                _speech.font.pixelSize = 40;
            }

            onSpeechStarted: {
                _speech.color = "green";
                _speech.font.pixelSize = 80;
            }

            onListenFinished: {
                _speech2.color = "grey";
                _speech2.font.pixelSize = 40;
            }

            onListenStarted: {
                _speech2.color = "green";
                _speech2.font.pixelSize = 80;
            }
        }

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

            Text{
                id: _speech
                objectName: "_speech"
                y: 250
                x: 75
                text: "...recording for 5 sec!"
                //font.pixelSize: 40
                //color: "grey"
                font.pixelSize: voice_cap_time
                color: textclr
                }

            Text{
                id: _speech2
                y: 500
                x: 75
                text: "...listening to voice-message!"
                font.pixelSize: voice_cap_time
                color: textclr
                }

            //timer ist wichtig für sequentiellen Ablauf
           Timer {
                   id: innerTimer
                   repeat: false
                   interval: 50

                   onTriggered: {
                       running = true;
                       _afbBackend.capture_voice();
                       _afbBackend.speechfinish();
                       _afbBackend.bt_voice_send();
                       /*
                       _root.seconds--;
                       if (_root.seconds == 0) {
                           running = false;
                           _root.seconds = _root.defaultSeconds
                           _speech.color = "grey"
                           _speech.font.pixelSize = 40;
                           _root.triggert()
                       }*/
                       running = false;
                   }
               }

           Timer {
                   id: innerTimer2
                   repeat: false
                   interval: 50

                   onTriggered: {
                       running = true;
                       _afbBackend.listen_msg();
                       _afbBackend.listenFinish();
                       running = false;
                   }
               }

            Rectangle{
                    id: _rect3
                    color: "grey"
                    y: 150
                    x: 100
                    width: 460
                    height: _mikropic.implicitHeight + 20
                    border.width: 5
                    border.color: "black"

                    Image {
                        id: _mikropic
                        anchors.left: _rect3.left
                        anchors.leftMargin: 10
                        anchors.top: _rect3.top
                        anchors.topMargin: 10
                        sourceSize.width: 250
                        sourceSize.height: 200
                        source: "qrc:/mikro.png"
                    }

                    /*
                    Button
                    {
                        id: _buttonRec
                        text: "Record"
                        font.pixelSize: 23
                        anchors.left: _mikropic.right
                        anchors.leftMargin: 20
                        anchors.top: _rect3.top
                        anchors.topMargin: 20
                        width: 150
                        height: 60

                        onClicked:
                        {
                            _afbBackend.teststart();
                            innerTimer.start();
                            //_afbBackend.capture_voice();
                        }
                    }

                    Button
                    {
                        id: _buttonRec2
                        text: "Stop"
                        font.pixelSize: 23
                        anchors.left: _buttonRec.right
                        anchors.leftMargin: 20
                        anchors.top: _rect3.top
                        anchors.topMargin: 20
                        width: 150
                        height: 60

                        onClicked:
                        {
                            _speech.font.pixelSize = 40;
                            _speech.color = "grey";
                        }
                    } */


                    Text{
                        anchors.left: _mikropic.right
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.top: _rect3.top
                        anchors.topMargin: 35
                        text: "press for recording"
                        font.pixelSize: 23
                    }


                    MouseArea{
                        id: _mouseArea1
                        anchors.fill: parent

                        onClicked:
                        {
                            _afbBackend.speechstart();
                            innerTimer.start();
                        }
                    }

            }


            Rectangle{
                    id: _rect4
                    color: "grey"
                    y: 400
                    x: 100
                    width: 460
                    height: _speakerpic.implicitHeight + 80
                    border.width: 5
                    border.color: "black"

                    Image {
                        id: _speakerpic
                        anchors.left: _rect4.left
                        anchors.leftMargin: 40
                        anchors.top: _rect4.top
                        anchors.topMargin: 40
                        sourceSize.width: 250
                        sourceSize.height: 200
                        source: "qrc:/lautsprecher.png"
                    }

                    Text{
                        anchors.left: _speakerpic.right
                        anchors.leftMargin: 50
                        anchors.rightMargin: 10
                        anchors.top: _rect4.top
                        anchors.topMargin: 35
                        text: "listen to voice-message"
                        font.pixelSize: 23
                    }

                    Button {
                        id: _msg_notification
                        visible: _afbBackend.Bth_msg_recv   // true or false
                        anchors.right: _rect4.right
                        anchors.top: _rect4.top
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
                    }

                    MouseArea{
                        id: _mouseArea2
                        anchors.fill: parent

                        onClicked:
                        {
                            _afbBackend.listenStart();
                            innerTimer2.start();
                        }
                    }

            }
 }
