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
                model: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '0']
                delegate: MoTalk_Key {
                    text: model.modelData
                    Layout.preferredWidth: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
                model: ['@', '#', '%', '&', '*', '-', '+', '(', ')']
                delegate: MoTalk_Key {
                    text: model.modelData
                    Layout.preferredWidth: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
                model: ['!', '"', '<', '>', '\'', ':', ';', '/']
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
                        var input = keyboard.target
                        if (input.cursorPosition > 0)
                            keyboard.target.remove(input.cursorPosition - 1, input.cursorPosition)
                    }
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            MoTalk_Key {
                text: 'ABC'
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
