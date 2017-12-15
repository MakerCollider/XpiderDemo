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

#define _USE_MATH_DEFINES

#include "xpider_center.h"

#include <math.h>
#include <QThread>
#include <QTime>
#include <QColor>

const quint16 XpiderCenter::kXpiderPort = 80;
const QString XpiderCenter::kXpiderHost = QString::fromStdString("192.168.100.1");

const quint16 XpiderCenter::kSendInterval = 20;
const quint16 XpiderCenter::kConnectInterval = 5000;
const quint16 XpiderCenter::kCommWatchdogInterval = 3000;

XpiderCenter::XpiderCenter(QObject *parent) : QObject(parent) {
  connected_ = false;
  is_active_ = false;
  last_step_counter_ = 0;
  start_autopilot_ = false;

  xpider_protocol_.Initialize(&xpider_info_);

  connect(&send_timer_, SIGNAL(timeout()), this, SLOT(SendBuffer()));
  connect(&comm_watchdog_, SIGNAL(timeout()), this, SLOT(CommWatchdog()));

  xpider_comm_.moveToThread(&comm_thread_);
  //connect(&comm_thread_, &QThread::finished, &xpider_comm_, &QObject::deleteLater);
  connect(this, &XpiderCenter::connectDevice, &xpider_comm_, &XpiderComm::Connect);
  connect(this, &XpiderCenter::disconnectDevice, &xpider_comm_, &XpiderComm::Disconnect);
  connect(&xpider_comm_, &XpiderComm::error, this, &XpiderCenter::CommErrorHandler);
  connect(&xpider_comm_, &XpiderComm::newFrame, this, &XpiderCenter::NewFrameHandler);
  connect(&xpider_comm_, &XpiderComm::connected, this, &XpiderCenter::deviceConnectedHandler);
  connect(&xpider_comm_, &XpiderComm::disconnected, this, &XpiderCenter::deviceDisconnectedHandler);
  comm_thread_.start();
}

XpiderCenter::~XpiderCenter() {
  if(connected_) {
    disconnectXpider();
  }
  comm_thread_.terminate();
  comm_thread_.wait();
}

void XpiderCenter::connectXpider() {
  if(connected_ == false && xpider_comm_.is_connecting() == false) {
    emit connectDevice(kXpiderHost, kXpiderPort);
    qDebug() << "(XpiderCenter) connectXpider";
  } else {
    qDebug() << "(XpiderCenter) xpider connected or connecting...";
  }
}

void XpiderCenter::disconnectXpider() {
  emit disconnectDevice();
}

void XpiderCenter::deviceConnectedHandler() {
  connected_ = true;
  is_active_ = true;

  if(!send_timer_.isActive()) {
    send_buffer_array_.clear();
    send_timer_.start(kSendInterval);
  }

  if(!comm_watchdog_.isActive()) {
    // comm_watchdog_.start(kCommWatchdogInterval);
  }

  emit deviceConnected();
  qDebug() << "(XpiderCenter) device connected";
}

void XpiderCenter::deviceDisconnectedHandler() {
  send_timer_.stop();
  comm_watchdog_.stop();

  connected_ = false;
  emit deviceDisconnected();
  qDebug() << "(XpiderCenter) device disconnected";
}

void XpiderCenter::CommErrorHandler(const QString &message) {
  qWarning() << "(XpiderCenter) Error:" << message;
}

void XpiderCenter::CommWatchdog() {
  if(is_active_ == false) {
    connected_ = false;
    comm_watchdog_.stop();
    emit disconnectDevice();
    qDebug() << "(XpiderCenter) Xpider comm watchdog triggered";
  } else {
    is_active_ = false;
  }
}

void XpiderCenter::PushSendBuffer(const QByteArray &data) {
  if(connected_) {
    send_buffer_array_.enqueue(data);
  } else {
    emit deviceDisconnected();
  }
}

void XpiderCenter::SendBuffer() {
  if(!send_buffer_array_.empty()) {
    xpider_comm_.SendFrame(send_buffer_array_.dequeue());
  }
}

void XpiderCenter::NewFrameHandler(QByteArray data) {
  is_active_ = true;

  const char* buffer_head = data.data();

  // qDebug() << "buffer_type: " << static_cast<int>(buffer_head[0]);

  xpider_protocol_.GetMessage(reinterpret_cast<const uint8_t*>(buffer_head), data.length());

  if(buffer_head[0] == XpiderProtocol::kRegisterResponse) {
    EmitInfoUpdated(buffer_head);
  }

  emit voltageUpdated(xpider_info_.voltage);
  emit distanceUpdated(xpider_info_.obstacle_distance);
  emit soundLevelUpdated(xpider_info_.sound_level);

  qDebug() << QString("Heart message: step_counter %1, obstacle_distance %2, voltage %3, heading %4, sound %5")\
                     .arg(xpider_info_.step_counter).arg(xpider_info_.obstacle_distance)\
                     .arg(xpider_info_.voltage).arg(xpider_info_.yaw_pitch_roll[0]).arg(xpider_info_.sound_level);
}

void XpiderCenter::EmitInfoUpdated(const char* buffer) {
  const char* value_string;
  QString value_qstring;

  // qDebug() << "emit info type: " << static_cast<int>(buffer[1]);

  switch(buffer[1]) {
    case XpiderProtocol::kRegName: {
      value_string = xpider_info_.custom_data_.name;
      value_qstring = QString(value_string);

      emit nameUpdated(value_qstring);
      qDebug() << "Get Register(Name): " << value_qstring;
      break;
    }
    case XpiderProtocol::kRegUUID: {
      value_qstring = QString::number(static_cast<long>(xpider_info_.custom_data_.uuid));
      emit uuidUpdated(value_qstring);
      qDebug() << "Get Register(UUID): " << value_qstring;
      break;
    }
    case XpiderProtocol::kRegHardwareVersion: {
      value_string = xpider_info_.custom_data_.hardware_version;
      value_qstring = QString(value_string);
      emit versionUpdated(value_qstring);
      qDebug() << "Get Register(Version): " << value_qstring;
      break;
    }
    case XpiderProtocol::kRegFirmwareVersion: {
      value_string = xpider_info_.firmware_version;
      value_qstring = QString(value_string);
      emit firmwareUpdated(value_qstring);
      qDebug() << "Get Register(Firmware): " << value_qstring;
      break;
    }
    case XpiderProtocol::kRegControllerVersion: {
      value_string = xpider_info_.controller_version;
      value_qstring = QString(value_string);
      qDebug() << "Get Register(Controller): " << value_qstring;
      emit controllerUpdated(value_qstring);
      break;
    }
  }
}

void XpiderCenter::getXpiderInfo() {
  uint8_t *buffer;
  uint16_t length;

  QByteArray temp;

  xpider_protocol_.GetRegister(XpiderProtocol::kRegName, &buffer, &length);
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);

  xpider_protocol_.GetRegister(XpiderProtocol::kRegUUID, &buffer, &length);
  temp.clear();
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);

  xpider_protocol_.GetRegister(XpiderProtocol::kRegHardwareVersion, &buffer, &length);
  temp.clear();
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);

  xpider_protocol_.GetRegister(XpiderProtocol::kRegFirmwareVersion, &buffer, &length);
  temp.clear();
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);

  xpider_protocol_.GetRegister(XpiderProtocol::kRegControllerVersion, &buffer, &length);
  temp.clear();
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);
}

void XpiderCenter::emergencyStop() {
  QByteArray temp_info;
  temp_info.append(XpiderProtocol::kEmergencyStop);
  temp_info.append(QByteArray::fromHex("FFFFFF"));

  PushSendBuffer(temp_info);
}

void XpiderCenter::setMove(int move, int rotate) {
  // qDebug() << "move: " << move << " rotate: " << rotate;
  if(abs(move) > 100 || abs(rotate) > 100) {
    qDebug() << "Invaild move & rotate value";
  } else {
    uint8_t *buffer;
    uint16_t length;
    xpider_info_.move = move;
    xpider_info_.rotate = rotate;
    xpider_protocol_.GetBuffer(xpider_protocol_.kMove, &buffer, &length);

    QByteArray temp;
    temp.append(reinterpret_cast<char*>(buffer), length);
    PushSendBuffer(temp);

    // qDebug() << "Setmove: " << temp.toHex() << ",length: " << temp.length();
  }
}

void XpiderCenter::setEyeAngle(int angle) {
  uint8_t *buffer;
  uint16_t length;

  xpider_info_.eye_angle = angle;
  xpider_protocol_.GetBuffer(xpider_protocol_.kEye, &buffer, &length);

  QByteArray temp;
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);
}

void XpiderCenter::setFrontLedsByHSV(int left_h, int left_s, int left_v,
                                     int right_h, int right_s, int right_v) {
  /* QColor s,v value is 0-255, standard hsv value is 0-360, 0-100, 0-100 */
  QColor left_color = QColor::fromHsv(left_h, left_s/100*255, left_v/100*255);
  QColor right_color = QColor::fromHsv(right_h, right_s/100*255, right_v/100*255);

  setFrontLeds(left_color.red(), left_color.green(), left_color.blue(),
               right_color.red(), right_color.green(), right_color.blue());
}

void XpiderCenter::setFrontLeds(int left_r, int left_g, int left_b,
                                int right_r, int right_g, int right_b) {
  uint8_t *buffer;
  uint16_t length;

  xpider_info_.left_led_rgb[0] = left_r;
  xpider_info_.left_led_rgb[1] = left_g;
  xpider_info_.left_led_rgb[2] = left_b;
  xpider_info_.right_led_rgb[0] = right_r;
  xpider_info_.right_led_rgb[1] = right_g;
  xpider_info_.right_led_rgb[2] = right_b;

  xpider_protocol_.GetBuffer(xpider_protocol_.kFrontLeds, &buffer, &length);

  QByteArray temp;
  temp.append(reinterpret_cast<char*>(buffer), length);
  PushSendBuffer(temp);
}
