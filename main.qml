/*
 * Xpider Demo, running on PC with QT5
 * Copyright (C) 2015-2017  Roboeve, MakerCollider
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
  visible: true
  width: 400
  height: 480
  title: qsTr("XpiderDemo")

  SwipeView {
    id: swipeView
    anchors.fill: parent
    currentIndex: tabBar.currentIndex

    ControlPage {
    }

    VideoPage {
    }
  }

  footer: TabBar {
    id: tabBar
    currentIndex: swipeView.currentIndex
    TabButton {
      text: qsTr("Control")
    }
    TabButton {
      text: qsTr("Video")
    }
  }

  Connections {
    target: xpider_center

    /* Xpider Connected */
    onDeviceConnected: {
      xpider_center.getXpiderInfo();
      console.debug("Xpider connected!");
    }

    /* Xpider Disconnected */
    onDeviceDisconnected: {
      xpider_center.connectXpider();
      console.debug("Device disconnected.");
    }
  }

  Component.onCompleted: {
    xpider_center.connectXpider();
  }
}
