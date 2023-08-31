#pragma once

#include <stdint.h>

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QVector>
#include <communication.hpp>

namespace Communication {

struct Frame {
  QVector<uint8_t> data;
  uint32_t id;
};

enum ImgType {
  ImgTypeUnknow = 0,
  ImgTypePng,
};

enum FrameType {
  FrameTypeUnknow = 0,
  FrameTypeImg,
  FrameTypeImgPoint,
  FrameTypePoint
};

struct FrameImg {
  QGraphicsPixmapItem *item;
  uint8_t draw_id;
};

bool decode(QByteArray &buff, QVector<struct Data> &data);

struct FrameImg decode_img(QVector<uint8_t> &data);

bool decode_img_points(uint8_t data, struct Frame &result);

QVector<float> *decode_points(QVector<uint8_t> &data);

bool decode_points_continues(uint8_t data, struct Frame &result);

}  // namespace Communication