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
import QtQuick.Window 2.0
import io.qt.BackendStuff 1.0

Item {
    id: root
    implicitWidth: Screen.width
    implicitHeight: Screen.height / 3
    signal show()
    signal hide()
    signal toggleMode()
    property Item target

    onTargetChanged:
    {
        var v = target && target.activeFocus && typeof(target.inputMethodComposing) !== 'undefined'
        if (v)
            show()
        else
            hide()
    }

}
