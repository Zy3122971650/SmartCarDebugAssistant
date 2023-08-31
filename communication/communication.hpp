#pragma once

#include <QObject>
#include <QSerialPort>
#include <QTcpSocket>
#include <QVector>

#include "frame.hpp"

namespace Communication {

enum ConnectType {
  Serial,
  Socket,
};

struct Data {
  QVector<struct FrameImg> img;
  QVector<float> *img_points;
  QVector<float> *points;
};

class communication_hal : public QObject {
  Q_OBJECT
 private slots:
  void callbackReadyDataSlot();
 signals:
  void communicationRecieve(const QByteArray &data);
  void frameDecodeFinished(void);

 public:
  // 理论上要作为私有变量保护，但是快速开发没办法啦
  QVector<struct Data> data;
  enum ConnectType connect_type;
  QSerialPort *serial_port;
  QTcpSocket *socket_port;

  communication_hal();
  ~communication_hal();
  bool open(enum ConnectType t, QString path, QString port_or_baud);
  void close();
  void write(QByteArray &buff);
};

}  // namespace Communication
