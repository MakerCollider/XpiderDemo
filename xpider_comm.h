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

#ifndef XPIDER_COMM_H_
#define XPIDER_COMM_H_

#include <QTimer>
#include <QObject>
#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>

#include "hdlc_qt.h"

class XpiderComm: public QObject {
  Q_OBJECT

public:
    static const QByteArray kDataHeader;
    static const quint16 kConnectTimeout;
    static const quint16 kHeartbeatInterval;

private:
    bool is_connecting_ = false;
    bool connected_;
    QString host_name_;
    quint16 host_port_;
    QTcpSocket *socket_ = NULL;

    HDLC_qt* hdlc_;

    bool heartbeat_;
    QTimer *heartbeat_timer_ = NULL;

public:
    XpiderComm(QObject *parent = 0);
    ~XpiderComm();

    bool is_connecting() { return is_connecting_; }

signals:
    void connected();
    void disconnected();
    void newFrame(const QByteArray &);
    void error(const QString &);

public slots:
    void Connect(const QString &host_name, const quint16 &host_port);
    void Disconnect();
    void SendFrame(const QByteArray &frame);

private slots:
    void HeartBeat();
    void ReadyRead();
    void SocketConnectedHanlder();
    void SocketDisconnectedHandler();
    void Send(QByteArray data);
    void GetFrame(QByteArray data, quint16 data_length);
    void ErrorHandler(QAbstractSocket::SocketError socket_error);
};

#endif // XPIDER_COMM_H_
