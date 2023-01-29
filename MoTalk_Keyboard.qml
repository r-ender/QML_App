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
import QtQuick.Controls 2.0

Item {
    id: root
    implicitWidth: Screen.width
    implicitHeight: Screen.height / 2

    property Item target
    visible: false

    Rectangle {
        anchors.fill: parent
        color: 'black'
        opacity: 0.5
    }

    property MoTalk_AbstractKeyboard active: alphabet
    MoTalk_Alphabet {
        id: alphabet
        anchors.fill: parent
        anchors.margins: parent.height / 20
        target: root.target
        visible: root.active === alphabet
        onShow: root.visible = true
        onHide: root.visible = false
        onToggleMode: root.active = symbols
    }

    MoTalk_Symbols {
        id: symbols
        anchors.fill: parent
        anchors.margins: parent.height / 20
        target: root.target
        visible: root.active === symbols
        onShow: root.visible = true
        onHide: root.visible = false
        onToggleMode: root.active = alphabet
    }
}
