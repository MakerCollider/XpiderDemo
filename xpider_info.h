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

#ifndef XPIDER_INFO_H_
#define XPIDER_INFO_H_

#include <stdint.h>
#include <string.h>

class XpiderInfo {
public:

  struct CustomData {
    char name[20];
    char hardware_version[20];
    uint32_t uuid;
  };

  static const char* kCustomDataFileName;
  static const CustomData kInitializeCustomData;

  int8_t move;
  int8_t rotate;
  int8_t count_speed;
  uint8_t count;

  uint8_t rotate_speed;
  float rotate_rad;
  uint8_t walk_speed;
  int8_t walk_step;

  uint16_t obstacle_distance;

  uint8_t left_led_rgb[3];
  uint8_t right_led_rgb[3];

  bool eye_power;
  int8_t eye_angle;

  bool autopilot_enable;
  float autopilot_heading;

  float voltage;
  uint16_t step_counter;

  float yaw_pitch_roll[3];

  char firmware_version[20];
  char controller_version[20];

  CustomData custom_data_;

  uint8_t sound_level;
};
#endif // XPIDER_INFO_H_
