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

import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Item {
  id: item1
  property alias forward: forward
  property alias turn_left: turn_left
  property alias turn_right: turn_right
  property alias backward: backward
  property alias led_slider: led_slider

  property alias obstacle_value: obstacle_value
  property alias name_value: name_value
  property alias voltage_value: voltage_value
  property alias sound_level: sound_level
  property alias controller_value: controller_value
  property alias firmware_value: firmware_value
  property alias version_value: version_value
  property alias uuid_value: uuid_value
  property alias eye_slider: eye_slider

  Button {
    id: forward
    x: 270
    y: 23
    text: qsTr("Forward")
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Button {
    id: turn_left
    x: 160
    y: 72
    text: qsTr("Left")
    anchors.horizontalCenterOffset: -100
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Button {
    id: turn_right
    x: 380
    y: 72
    text: qsTr("Right")
    anchors.horizontalCenterOffset: 100
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Button {
    id: backward
    x: 271
    y: 127
    text: qsTr("Backward")
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Slider {
    id: led_slider
    x: 168
    y: 223
    width: 304
    height: 14
    anchors.horizontalCenterOffset: 0
    anchors.horizontalCenter: parent.horizontalCenter
    value: 0.5
  }

  Text {
    id: text1
    x: 199
    y: 188
    text: qsTr("Press and hold the button to control xpider")
    anchors.horizontalCenterOffset: 0
    anchors.horizontalCenter: parent.horizontalCenter
    font.pixelSize: 12
  }

  Slider {
    id: eye_slider
    x: 168
    y: 255
    width: 304
    height: 29
    anchors.horizontalCenterOffset: 0
    anchors.horizontalCenter: parent.horizontalCenter
    value: 0.5
  }

  Grid {
    id: grid
    x: 170
    y: 304
    width: 253
    height: 123
    anchors.horizontalCenterOffset: -10
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: 11
    layoutDirection: Qt.LeftToRight
    flow: Grid.TopToBottom
    rows: 4
    columns: 4

    Text {
      id: name_text
      text: qsTr("Name:")
      font.bold: true
      verticalAlignment: Text.AlignTop
      horizontalAlignment: Text.AlignLeft
      font.pixelSize: 16
    }

    Text {
      id: obstacle_text
      text: qsTr("Obstacle:")
      font.bold: true
      font.pixelSize: 16
    }

    Text {
      id: voltage_text
      text: qsTr("Voltage:")
      font.bold: true
      font.pixelSize: 16
    }

    Text {
      id: sound_text
      text: qsTr("Sound:")
      font.bold: true
      font.pixelSize: 16
    }

    Text {
      id: name_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }


    Text {
      id: obstacle_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }

    Text {
      id: voltage_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }

    Text {
      id: sound_level
      text: qsTr("NULL")
      font.pixelSize: 16
    }


    Text {
      id: uuid_text
      text: qsTr("UUID:")
      font.bold: true
      font.pixelSize: 16
    }


    Text {
      id: version_text
      text: qsTr("Version")
      font.bold: true
      font.pixelSize: 16
    }

    Text {
      id: firmware_text
      text: qsTr("Firmware:")
      font.bold: true
      font.pixelSize: 16
    }


    Text {
      id: controller_text
      text: qsTr("Controller")
      font.bold: true
      font.pixelSize: 16
    }


    Text {
      id: uuid_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }


    Text {
      id: version_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }

    Text {
      id: firmware_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }

    Text {
      id: controller_value
      text: qsTr("NULL")
      font.pixelSize: 16
    }

  }

  Text {
    id: text2
    x: 115
    y: 223
    text: qsTr("LED")
    anchors.verticalCenter: led_slider.verticalCenter
    anchors.right: led_slider.left
    anchors.rightMargin: 10
    font.pixelSize: 12
  }

  Text {
    id: text3
    x: 115
    y: 262
    text: qsTr("Eye")
    anchors.verticalCenter: eye_slider.verticalCenter
    anchors.right: eye_slider.left
    anchors.rightMargin: 10
    font.pixelSize: 12
  }

  Text {
      id: text4
      y: 268
      text: qsTr("Eye")
      anchors.verticalCenter: eye_slider.verticalCenter
      anchors.left: eye_slider.right
      anchors.leftMargin: 10
      font.pixelSize: 12
  }

  Text {
      id: text5
      y: 223
      text: qsTr("LED")
      anchors.verticalCenter: led_slider.verticalCenter
      anchors.left: eye_slider.right
      anchors.leftMargin: 10
      font.pixelSize: 12
  }
}
