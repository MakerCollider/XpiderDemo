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

#include "xpider_protocol.h"

XpiderProtocol::XpiderProtocol() {
}

void XpiderProtocol::Initialize(XpiderInfo* xpider_info) {
  xpider_info_ = xpider_info;
}

void XpiderProtocol::GetBuffer(const MessageType &index, uint8_t** send_buffer, uint16_t *length) {
  switch (index) {
    case kHeartBeat: {
      *length = 20;

      send_msg_buffer_[0] = kHeartBeat;
      memcpy(&send_msg_buffer_[1], &xpider_info_->step_counter, 2);
      memcpy(&send_msg_buffer_[3], &xpider_info_->obstacle_distance, 2);

      int16_t temp_voltage = static_cast<int16_t>(xpider_info_->voltage*100.0f);
      memcpy(&send_msg_buffer_[5], &temp_voltage, 2);

      memcpy(&send_msg_buffer_[7], xpider_info_->yaw_pitch_roll, sizeof(float)*3);
      send_msg_buffer_[19] = xpider_info_->sound_level;
      
      break;
    }
    case kEye: {
      *length = 3;
      
      send_msg_buffer_[0] = kEye;
      send_msg_buffer_[1] = xpider_info_->eye_power;
      send_msg_buffer_[2] = xpider_info_->eye_angle;
      
      break;
    }
    case kFrontLeds: {
      *length = 7;
      
      send_msg_buffer_[0] = kFrontLeds;
      memcpy(&send_msg_buffer_[1], &xpider_info_->left_led_rgb, 3);
      memcpy(&send_msg_buffer_[4], &xpider_info_->right_led_rgb, 3);
      
      break;
    }
    case kMove: {
      *length = 3;

      send_msg_buffer_[0] = kMove;
      send_msg_buffer_[1] = xpider_info_->move;
      send_msg_buffer_[2] = xpider_info_->rotate;

      break;
    }
    case kStep: {
      *length = 3;

      send_msg_buffer_[0] = kStep;
      send_msg_buffer_[1] = xpider_info_->count_speed;
      send_msg_buffer_[2] = xpider_info_->count;

      break;
    }
    case kAutoMove: {
      *length = 8;

      send_msg_buffer_[0] = kAutoMove;
      send_msg_buffer_[1] = xpider_info_->rotate_speed;
      memcpy(&send_msg_buffer_[2], &xpider_info_->rotate_rad, 4);
      send_msg_buffer_[6] = xpider_info_->walk_speed;
      send_msg_buffer_[7] = static_cast<uint8_t>(xpider_info_->walk_step);

      break;
    }
    case kAutoPilot: {
      if(xpider_info_->autopilot_enable) {
        *length = 4;

        int16_t temp_autopilot = static_cast<int16_t>(xpider_info_->autopilot_heading*100.0f);
        memcpy(&send_msg_buffer_[2], &temp_autopilot, 2);
      } else {
        *length = 2;
      }

      send_msg_buffer_[0] = kAutoPilot;
      send_msg_buffer_[1] = xpider_info_->autopilot_enable;

      break;
    }
    case kEmergencyStop: {
      *length = 1;

      send_msg_buffer_[0] = kEmergencyStop;
      break;
    }
    case kUnknown:
    default: {
    }
  }

  *send_buffer = send_msg_buffer_;
}

XpiderProtocol::MessageType XpiderProtocol::GetMessage(const uint8_t *buffer, const uint16_t &length) {
  MessageType index = static_cast<MessageType>(buffer[0]);
  switch (index) {
    case kHeartBeat: {
      memcpy(&xpider_info_->step_counter, &buffer[1], 2);
      memcpy(&xpider_info_->obstacle_distance, &buffer[3], 2);

      int16_t temp_voltage;
      memcpy(&temp_voltage, &buffer[5], 2);
      xpider_info_->voltage = static_cast<float>(temp_voltage/100.0f);

      memcpy(xpider_info_->yaw_pitch_roll, buffer+7, sizeof(float)*3);

      xpider_info_->sound_level = buffer[19];
      break;
    }
    case kEye: {
      xpider_info_->eye_power = buffer[1];
      xpider_info_->eye_angle = buffer[2];
      break;
    }
    case kFrontLeds: {
      memcpy(&xpider_info_->left_led_rgb, &buffer[1], 3);
      memcpy(&xpider_info_->right_led_rgb, &buffer[4], 3);
      break;
    }
    case kMove: {
      xpider_info_->move = buffer[1];
      xpider_info_->rotate = buffer[2];
      break;
    }
    case kStep: {
      xpider_info_->count_speed = buffer[1];
      xpider_info_->count = buffer[2];
      break;
    }
    case kAutoMove: {
      xpider_info_->rotate_speed = buffer[1];
      memcpy(&xpider_info_->rotate_rad, &buffer[2], 4);
      xpider_info_->walk_speed = buffer[6];
      xpider_info_->walk_step = static_cast<int8_t>(buffer[7]);
      break;
    }
    case kAutoPilot: {
      xpider_info_->autopilot_enable = buffer[1];
      if(xpider_info_->autopilot_enable) {
        int16_t temp_autopilot;
        memcpy(&temp_autopilot, &buffer[2], 2);
        xpider_info_->autopilot_heading = static_cast<float>(temp_autopilot/100.0f);
      }
      break;
    }
    case kRun: {
      break;
    }
    case kEmergencyStop: {
      break;
    }
    case kGetRegister: {
      /* do nothing here */
      break;
    }
    case kUpdateRegister:
    case kRegisterResponse: {
      switch(buffer[1]) {
        case kRegUUID: {
          memcpy(&xpider_info_->custom_data_.uuid, &buffer[3], buffer[2]);
          break;
        }
        case kRegName: {
          memcpy(&xpider_info_->custom_data_.name, &buffer[3], buffer[2]);
          break;
        }
        case kRegHardwareVersion: {
          memcpy(&xpider_info_->custom_data_.hardware_version, &buffer[3], buffer[2]);
          break;
        }
        case kRegFirmwareVersion: {
          memcpy(&xpider_info_->firmware_version, &buffer[3], buffer[2]);
          break;
        }
        case kRegControllerVersion: {
          memcpy(&xpider_info_->controller_version, &buffer[3], buffer[2]);
          break;
        }
        default: {
        }
      }
      break;
    }
    default: {
      break;
    }
  }

  return index;
}

void XpiderProtocol::GetRegister(const RegIndex &register_index, uint8_t **send_buffer, uint16_t *length) {
  *length = 2;
  uint8_t *buffer = new uint8_t[2];

  buffer[0] = kGetRegister;
  buffer[1] = register_index;

  *send_buffer = buffer;
}

void XpiderProtocol::UpdateRegister(const RegIndex &register_index, uint8_t **send_buffer, uint16_t *length) {
  send_msg_buffer_[0] = kUpdateRegister;
  switch(register_index) {
    case kRegUUID: {
      send_msg_buffer_[1] = kRegUUID;
      send_msg_buffer_[2] = 4;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.uuid, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegName: {
      send_msg_buffer_[1] = kRegName;
      send_msg_buffer_[2] = strlen(xpider_info_->custom_data_.name)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.name, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegHardwareVersion: {
      send_msg_buffer_[1] = kRegHardwareVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->custom_data_.hardware_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.hardware_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegFirmwareVersion: {
      send_msg_buffer_[1] = kRegFirmwareVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->firmware_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->firmware_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegControllerVersion: {
      send_msg_buffer_[1] = kRegControllerVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->controller_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->controller_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
  }

  uint8_t* buffer = new uint8_t[*length];
  memcpy(buffer, send_msg_buffer_, *length);
  *send_buffer = buffer;
}

void XpiderProtocol::RegisterResponse(const RegIndex &register_index, uint8_t **send_buffer, uint16_t *length) {
  send_msg_buffer_[0] = kRegisterResponse;

  switch(register_index) {
    case kRegUUID: {
      send_msg_buffer_[1] = kRegUUID;
      send_msg_buffer_[2] = 4;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.uuid, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegName: {
      send_msg_buffer_[1] = kRegName;
      send_msg_buffer_[2] = strlen(xpider_info_->custom_data_.name)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.name, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegHardwareVersion: {
      send_msg_buffer_[1] = kRegHardwareVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->custom_data_.hardware_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->custom_data_.hardware_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegFirmwareVersion: {
      send_msg_buffer_[1] = kRegFirmwareVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->firmware_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->firmware_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
    case kRegControllerVersion: {
      send_msg_buffer_[1] = kRegControllerVersion;
      send_msg_buffer_[2] = strlen(xpider_info_->controller_version)+1;
      memcpy(&send_msg_buffer_[3], &xpider_info_->controller_version, send_msg_buffer_[2]);
      *length = 3 + send_msg_buffer_[2];
      break;
    }
  }

  uint8_t *buffer = new uint8_t[*length];
  memcpy(buffer, send_msg_buffer_, *length);
  *send_buffer = buffer;
}
