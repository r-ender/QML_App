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
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

MoTalk_AbstractKeyboard {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.height / 10
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                model: ['q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p']
                delegate: MoTalk_Key {
                    text: model.modelData
                    Layout.preferredWidth: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    capital: shift.checked
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Item {
                Layout.preferredWidth: 1
                Layout.fillWidth: true
            }

            Repeater {
                model: ['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l']
                delegate: MoTalk_Key {
                    text: model.modelData
                    Layout.preferredWidth: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    capital: shift.checked
                }
            }
            Item {
                Layout.preferredWidth: 1
                Layout.fillWidth: true
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            MoTalk_Key {
                id: shift
                image: './images/Keyboard_Shift.svg'
                checkable: true
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            Repeater {
                model: ['z', 'x', 'c', 'v', 'b', 'n', 'm', '@']
                delegate: MoTalk_Key {
                    text: model.modelData
                    Layout.preferredWidth: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    capital: shift.checked
                }
            }
            MoTalk_Key {
                image: './images/Keyboard_Back.svg'
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    if (!clearSelctedText()) {
                        var input = MoTalk_Keyboard.target
                        if (input.cursorPosition > 0)
                            MoTalk_Keyboard.target.remove(input.cursorPosition - 1, input.cursorPosition)
                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            MoTalk_Key {
                text: '123'
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: root.toggleMode()
            }
            MoTalk_Key {
                text: '.'
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            MoTalk_Key {
                text: ' '
                image: './images/Keyboard_Space.svg'
                Layout.preferredWidth: 12
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            MoTalk_Key {
                text: '!?'
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: root.toggleMode()
            }
            MoTalk_Key {
                image: './images/Keyboard_Arrow.svg'
                Layout.preferredWidth: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    root.hide()
                }
            }
        }
    }
}
