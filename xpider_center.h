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

#ifndef XPIDER_CENTER_H_
#define XPIDER_CENTER_H_

#include <QTimer>
#include <QMutex>
#include <QQueue>
#include <QPointF>
#include <QObject>

#include <vector>
#include <QVector>

#include "xpider_info.h"
#include "xpider_comm.h"
#include "xpider_protocol.h"

#define XPIDER_STEP_PIXEL 3.25 // cm per step
#define SPIDER_AUTOPILOT_MIN_RADIUS 200

class XpiderCenter : public QObject {
  Q_OBJECT

public:
  static const QString kXpiderHost;                /* Xpider host address*/
  static const quint16 kXpiderPort;                /* Xpider port */

  static const quint16 kSendInterval;
  static const quint16 kConnectInterval;
  static const quint16 kCommWatchdogInterval;

private:
  QThread comm_thread_;
  XpiderComm xpider_comm_;

  XpiderInfo xpider_info_;
  XpiderProtocol xpider_protocol_;

  QTimer send_timer_;
  QTimer comm_watchdog_;
  QQueue<QByteArray> send_buffer_array_;

  bool start_autopilot_;
  bool connected_;
  bool is_active_;

  uint16_t last_step_counter_;

public:
  explicit XpiderCenter(QObject *parent = 0);
  ~XpiderCenter();

  Q_INVOKABLE bool connected() { return connected_; }

  Q_INVOKABLE void connectXpider();
  Q_INVOKABLE void disconnectXpider();
  Q_INVOKABLE void getXpiderInfo();
  Q_INVOKABLE void emergencyStop();
  Q_INVOKABLE void setMove(int move, int rotate);
  Q_INVOKABLE void setEyeAngle(int angle);
  Q_INVOKABLE void setFrontLedsByHSV(int left_h, int left_s, int left_v,
                                     int right_h, int right_s, int right_v);
  Q_INVOKABLE void setFrontLeds(int left_r, int left_g, int left_b,
                                int right_r, int right_g, int right_b);

private:
  void EmitInfoUpdated(const char* buffer);

  void PushSendBuffer(const QByteArray &data);

  void UpdateAutopilot();
  void UpdateXpiderLocation();
  void UpdateTargetLocation();
  void UpdateObstacleLocation();

signals:
  void connectDevice(const QString &host_name, const quint16 &host_port);
  void disconnectDevice();
  void deviceConnected();
  void deviceDisconnected();

  void obstacleUpdated();
  void voltageUpdated(float voltage);
  void distanceUpdated(float distance);
  void soundLevelUpdated(int sound_value);
  void mapUpdated(bool autopilot_enable, float heading, QVariantList map_object_list);

  void uuidUpdated(QString uuid);
  void nameUpdated(QString name);
  void versionUpdated(QString version);
  void firmwareUpdated(QString firmware);
  void controllerUpdated(QString controller);

public slots:
  void NewFrameHandler(QByteArray);

private slots:
  void SendBuffer();
  void CommErrorHandler(const QString &message);
  void CommWatchdog();

  void deviceConnectedHandler();
  void deviceDisconnectedHandler();

};

#endif // XPIDER_CENTER_H_
