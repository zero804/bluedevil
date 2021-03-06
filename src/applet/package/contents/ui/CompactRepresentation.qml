/*
    SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

import QtQuick 2.2
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3

MouseArea {
    readonly property bool inPanel: (plasmoid.location == PlasmaCore.Types.TopEdge
        || plasmoid.location == PlasmaCore.Types.RightEdge
        || plasmoid.location == PlasmaCore.Types.BottomEdge
        || plasmoid.location == PlasmaCore.Types.LeftEdge)

    Layout.maximumWidth: inPanel ? units.iconSizeHints.panel : -1
    Layout.maximumHeight: inPanel ? units.iconSizeHints.panel : -1

    onClicked: plasmoid.expanded = !plasmoid.expanded

    hoverEnabled: true

    PlasmaCore.IconItem {
        id: bluetoothIcon
        anchors.fill: parent
        source: plasmoid.icon
        active: parent.containsMouse

        PlasmaComponents3.BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: runningActions > 0
            visible: running
        }
    }
}
