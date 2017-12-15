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

#include "rak_camera.h"

#include <QDebug>

RakCamera::RakCamera(QObject *parent) : QObject(parent) {
  initialized_ = false;
  is_running_ = false;
  work_flag_ = false;

  codec_ = NULL;
  codec_context_ = NULL;
  input_format_context_ = NULL;
  video_stream_index_ = -1;
}

RakCamera::~RakCamera() {
}

void RakCamera::Initialize(QMutex* mutex, std::string url) {
  url_ = url;
  mutex_ = mutex;
}

void RakCamera::stopCamera() {
  work_flag_ = false;
}

bool RakCamera::start() {
  if(avformat_open_input(&input_format_context_, url_.c_str(), NULL, NULL) != 0) {
    emit connectionResult(1);
    return false;
  }

  if(avformat_find_stream_info(input_format_context_, NULL) < 0) {
    emit connectionResult(2);
    return false;
  }

  for(uint i=0; i<input_format_context_->nb_streams; i++) {
    if(input_format_context_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      video_stream_index_ = i;
      break;
    }
  }

  if(video_stream_index_ == -1) {
    emit connectionResult(3);
    return false;
  }

  auto* codecpar = input_format_context_->streams[video_stream_index_]->codecpar;
  codec_ = avcodec_find_decoder(codecpar->codec_id);
  if(codec_ == NULL) {
    emit connectionResult(4);
    return false;
  }

  codec_context_ = avcodec_alloc_context3(codec_);

  if(avcodec_parameters_to_context(codec_context_, codecpar) != 0) {
    emit connectionResult(5);
    return false;
  }

  if(avcodec_open2(codec_context_, codec_, NULL) < 0) {
    emit connectionResult(6);
    return false;
  }

  emit connectionResult(0);
  return true;
}

bool RakCamera::stop() {
  avcodec_close(codec_context_);
  avformat_close_input(&input_format_context_);

  codec_ = NULL;
  codec_context_ = NULL;
  input_format_context_ = NULL;
  video_stream_index_ = -1;

  return true;
}

void RakCamera::doWork() {  
  av_register_all();
  avformat_network_init();

  if(!start()) {
    qDebug() << "(RakCamera) doWork failed";
    stop();
    return;
  }

  AVPacket av_packet;
  AVFrame* av_frame = av_frame_alloc();

  is_running_ = true;
  work_flag_ = true;
  while(work_flag_ == true) {
    if (av_read_frame(input_format_context_, &av_packet) < 0) {
      emit error(10);
      continue;
    }

    if(av_packet.stream_index != video_stream_index_) {
      // emit error(11);
      continue;
    }

    if(avcodec_send_packet(codec_context_, &av_packet) < 0) {
      emit error(12);
      continue;
    }

    if(avcodec_receive_frame(codec_context_, av_frame) < 0) {
      emit error(13);
      continue;
    }

    emit imageReady(av_frame->data[0], av_frame->data[1], av_frame->data[2]);
  }

  av_free(av_frame);
  stop();
  is_running_ = false;
}
