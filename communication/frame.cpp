#include "frame.hpp"
// todo： 重新设计存储方案
bool Communication::decode(QByteArray &buff, struct Data *data) {
  static bool isHeader = false;
  static uint8_t id = 0;
  static uint8_t frameType = 0;
  static uint8_t lenGroup = 0;
  static uint8_t count = 0;
  static uint8_t status = 0;
  static void *len = nullptr;
  static QVector<uint8_t> frameData;
  static bool success = false;
  success = false;
  foreach (uint8_t u8, buff) {
    if (!isHeader) {
      isHeader = (u8 == 0xAA);
      continue;
    }
    switch (status) {
      case 0:
        // 包序号
        if (id != u8 || data->img.length() == 0) {
          QVector<FrameImg> t;
          data->img.append(t);
        };
        id = u8;
        ++status;
        break;
      case 1:
        // 包类型
        frameType = u8;
        ++status;
        break;
      case 2:
        lenGroup = u8;
        count = u8;
        ++status;
        break;
      case 3:
        // 构造数据长度
        switch (lenGroup) {
          case 1:
            len = new uint8_t;
            *(uint8_t *)len = u8;
            ++status;
            break;
          case 2:
            if (!len) {
              len = new uint16_t;
              *(uint16_t *)len = 0;
            }
            {
              uint16_t temp = u8;
              *(uint16_t *)len |= (temp << (--count) * 8);
            }
            if (!count) ++status;
            break;
          case 4:
            if (!len) {
              len = new uint32_t;
              *(uint32_t *)len = 0;
            }
            {
              uint32_t temp = u8;
              *(uint32_t *)len |= (temp << (--count) * 8);
            }
            if (!count) ++status;
            break;
        }
        break;
      case 4:
        // 收集数据
        switch (lenGroup) {
          case 1:
            frameData.append(u8);
            --*(uint8_t *)len;
            if (*(uint8_t *)len == 0) ++status;

            break;
          case 2:

            frameData.append(u8);
            --*(uint16_t *)len;
            if (*(uint16_t *)len == 0) ++status;

            break;
          case 4:
            frameData.append(u8);

            --*(uint32_t *)len;
            if (*(uint32_t *)len == 0) ++status;
            break;
        }
        // 解析数据
        if (status == 5) {
          switch (frameType) {
            case FrameType::FrameTypeImg:
              data->img.last().append(decode_img(frameData));
              break;
            case FrameType::FrameTypePoint:
              data->points.append(decode_points(frameData));
              break;
            default:
              break;
          }
          // clear
          isHeader = false;
          id = 0;
          frameType = 0;
          lenGroup = 0;
          count = 0;
          status = 0;
          switch (lenGroup) {
            case 1:

              delete (uint8_t *)len;
              break;
            case 2:

              delete (uint16_t *)len;

              break;
            case 4:

              delete (uint32_t *)len;
              break;
          }
          len = nullptr;
          frameData.clear();
          //
          success = true;
        }
        break;
    }
  }
  return success;
}

struct Communication::FrameImg Communication::decode_img(
    QVector<uint8_t> &data) {
  struct FrameImg frame_img;
  frame_img.draw_id = data[0];
  uint8_t img_type = data[1];  // TODO: 暂时自动配置
  uint32_t width = *(uint32_t *)(&data[2]);
  uint32_t height = *(uint32_t *)(&data[6]);
  QImage image;

  switch (img_type) {
    case ImgTypePng:
      image.loadFromData(&data[10], data.length() - 1, nullptr);
      break;
    case ImgTypeRgb:
      // todo: 更新帧协议
      image = QImage(&data[10], width, height, QImage::Format_RGB888);
      break;
    case ImgTypeGray:
      image = QImage(&data[10], width, height, QImage::Format_Grayscale8);
      break;
    default:
      break;
  }
  QPixmap pixmap = QPixmap::fromImage(image);
  frame_img.item = new QGraphicsPixmapItem(pixmap);
  return frame_img;
}

bool Communication::decode_img_points(uint8_t data, struct Frame &result) {}

QVector<float> Communication::decode_points(QVector<uint8_t> &data) {
  // TODO: 这里需要确定大小端，好的做法是：先确定大小端，然后对每组数据取反
  QVector<float> f;
  f.append(*(float *)&data[0]);
  f.append(*(float *)&data[4]);
  f.append(*(float *)&data[8]);
  f.append(*(float *)&data[12]);
  f.append(*(float *)&data[16]);
  f.append(*(float *)&data[20]);
  f.append(*(float *)&data[24]);
  f.append(*(float *)&data[28]);
  f.append(*(float *)&data[32]);
  return f;
}

bool Communication::decode_points_continues(uint8_t data,
                                            struct Frame &result) {}