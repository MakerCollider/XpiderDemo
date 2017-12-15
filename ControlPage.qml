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
import QtQuick.Controls 2.0

ControlPageForm {
  forward.onPressAndHold: {
    xpider_center.setMove(100, 0);
  }
  forward.onReleased: {
    xpider_center.setMove(0, 0);
  }

  backward.onPressAndHold: {
    xpider_center.setMove(-100, 0);
  }
  backward.onReleased: {
    xpider_center.setMove(0, 0);
  }

  turn_left.onPressAndHold: {
    xpider_center.setMove(0, -100);
  }
  turn_left.onReleased: {
    xpider_center.setMove(0, 0);
  }

  turn_right.onPressAndHold: {
    xpider_center.setMove(0, 100);
  }
  turn_right.onReleased: {
    xpider_center.setMove(0, 0);
  }

  led_slider.onValueChanged: {
    console.debug("Led Slider value is ", led_slider.value);
    xpider_center.setFrontLedsByHSV(led_slider.value*255.0, 100, 100, led_slider.value*255.0, 100, 100);
  }

  eye_slider.onValueChanged: {
    console.debug("Eye Slider value is ", led_slider.value);
    xpider_center.setEyeAngle(eye_slider.value*60);
  }

  Connections {
    target: xpider_center

    onNameUpdated: {
      name_value.text = name;
    }

    onVersionUpdated: {
      version_value.text = version;
    }

    onFirmwareUpdated: {
      firmware_value.text = firmware
    }

    onUuidUpdated: {
      uuid_value.text = uuid;
    }

    onControllerUpdated: {
      controller_value.text = controller
    }

    onVoltageUpdated: {
      voltage_value.text = voltage.toFixed(1) + "v";
    }

    onDistanceUpdated: {
      obstacle_value.text = distance.toFixed(1) + "mm";
    }

    onSoundLevelUpdated: {
      sound_level.text = sound_value;
    }
  }
}
