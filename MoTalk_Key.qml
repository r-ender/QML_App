/*
 * Copyright (C) 2016 The Qt Company Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0


MouseArea {
    id: root
    implicitWidth: 50
    implicitHeight: 80
    property string keyPressed
    property string text

    Rectangle {
        anchors.fill: parent
        border.width: 2
        border.color: 'white'
        smooth: true
        radius: root.height / 10
        color: 'gray'
        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            opacity: root.pressed || root.checked ? 0 : 0.5
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: 'transparent'
                }
                GradientStop {
                    position: 1.0
                    color: '#66FF99'
                }
            }
        }
    }

    Text {
        id: label
        anchors.centerIn: parent
        color: 'white'
        //font.pixelSize: root.height / 2
        font.pixelSize: root.width / 2
        //text: root.capital ? root.text.toUpperCase() : root.text.toLowerCase()
        text: root.text.toUpperCase()
    }
}


/*
import QtQuick 2.6
import QtQuick.Layouts 1.1
import io.qt.BackendStuff 1.0
import QtQuick.Controls 2.0


MouseArea {
    id: root
    implicitWidth: 500
    implicitHeight: 800
    property string keyPressed
    property string text
    property alias image: image.source
    //property string labeltext: _label.text
    property bool checkable: false
    property bool checked: false
    property bool capital: false


    BackendStuff {
        id: _afbBackend
    }

    onClicked: {
        if (checkable) {
            checked = !checked
        } else {
            console.log("_label.text:", _label.text)
            if (_label.text.length === 1)
                _afbBackend.receiveKeypress = _label.text
                insert(_label.text)
        }
    }


    function clearSelctedText() {
    //console.log("clear selected text")
    if (_keyboard.target === null) { return true}
        else
    {
        var input = _keyboard.target
        if (input.selectedText.length > 0) {
            input.remove(input.selectionStart, input.selectionEnd)
            return true
        }
        return false
    }
    }

    function insert(text) {
        clearSelctedText()
        var input = _keyboard.target
        _afbBackend.sendKeyboardInput(_label.text)
        //_keyboard.target.insert(input.cursorPosition, text)
    }

    Rectangle {
        anchors.fill: parent
        border.width: 2
        border.color: 'white'
        smooth: true
        radius: root.height / 10
        color: 'gray'
        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            opacity: root.pressed || root.checked ? 0 : 0.5
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: 'transparent'
                }
                GradientStop {
                    position: 1.0
                    color: '#66FF99'
                }
            }
        }
    }


    Text {
        id: _label
        anchors.centerIn: parent
        color: 'white'
        font.pixelSize: root.height / 2
        text: root.capital ? root.text.toUpperCase() : root.text.toLowerCase()
    }


    Image {
        id: image
        anchors.centerIn: parent
        scale: 0.8
    }
}
*/
