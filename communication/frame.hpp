#pragma once

#include <stdint.h>

#include <QVector>

namespace Communication {

struct Frame {
  QVector<uint8_t> data;
  uint32_t id;
};

void decode(uint8_t *buff);

void decode_img(uint8_t data);

void decode_img_points(uint8_t data);

void decode_points(uint8_t data);

void decode_points_continues(uint8_t data);

}  // namespace Communication