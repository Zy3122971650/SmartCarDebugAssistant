#pragma once

#include <stdint.h>

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QVector>

namespace Communication {

struct Frame {
  QVector<uint8_t> data;
  uint32_t id;
};

enum ImgType {
  ImgTypeUnknow = 0,
  ImgTypePng,
  ImgTypeRgb,
  ImgTypeGray,
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

struct Data {
  QVector<QVector<struct FrameImg>> img;
  QVector<float> *img_points;
  QVector<QVector<float>> points;
};

bool decode(QByteArray &buff, struct Data *data);

struct FrameImg decode_img(QVector<uint8_t> &data);

bool decode_img_points(uint8_t data, struct Frame &result);

QVector<float> decode_points(QVector<uint8_t> &data);

bool decode_points_continues(uint8_t data, struct Frame &result);

}  // namespace Communication