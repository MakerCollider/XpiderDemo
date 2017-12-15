/*
 * Xpider APP software, running on both ios and android
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

#ifndef RAK_CAMERA_H_
#define RAK_CAMERA_H_

#include <QObject>
#include <QMutex>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
}
#endif

class RakCamera : public QObject {
  Q_OBJECT
public:
  explicit RakCamera(QObject *parent = 0);
  ~RakCamera();

  bool is_running() { return is_running_; }

  void Initialize(QMutex* mutex, std::string url);
  void stopCamera();

  bool start();

private:
  bool work_flag_;
  bool is_running_;
  bool initialized_;

  std::string url_;

  QMutex* mutex_;

  AVCodec *codec_;
  AVCodecContext* codec_context_;
  AVFormatContext* input_format_context_;

  int video_stream_index_;

  bool stop();

signals:
  void imageReady(uint8_t* data, uint8_t* data1, uint8_t* data2);
  void connectionResult(int result_code);
  void error(int error_code);

public slots:
  void doWork();
};

#endif // RAK_CAMERA_H_
