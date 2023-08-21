#include "communication.hpp"
using namespace Communication;
bool communication_hal::open(enum ConnectType t, QString path,
                             QString port_or_baud) {
  qDebug() << "打开连接: " << t;
  qDebug() << "路径: : " << path;
  qDebug() << "端口或速率: " << port_or_baud;
  switch (t) {
    case ConnectType::Serial: {
      QSerialPort *port = new QSerialPort();
      port->setPortName(path);
      port->setBaudRate(port_or_baud.toInt());
      if (!port->open(QIODeviceBase::ReadWrite)) {
        delete port;
        return false;
      }
      this->connect_type = t;
      this->serial_port = port;
      QObject::connect(this->serial_port, SIGNAL(readyRead()), this,
                       SLOT(callbackReadyDataSlot()));
    } break;
    case ConnectType::Socket:
      return false;
    default:
      return false;
  }
  return true;
}
void communication_hal::close() {
  switch (this->connect_type) {
    case ConnectType::Serial:
      this->serial_port->close();
      delete this->serial_port;
      this->serial_port = nullptr;
      break;

    default:
      break;
  }
}

void communication_hal::callbackReadyDataSlot() {
  qDebug() << "数据准备好了";
  QByteArray data = this->serial_port->readAll();
  qDebug() << "数据是：" << data;

  // TODO: 设计一个信号，把数据传递到显示窗口
  emit communicationRecieve(data);
}
communication_hal::communication_hal() {
  this->socket_port = nullptr;
  this->serial_port = nullptr;
  this->connect_type = ConnectType::Serial;
}
communication_hal::~communication_hal() {}
void communication_hal::write(QByteArray &buff) {
  switch (this->connect_type) {
    case ConnectType::Serial:
      this->serial_port->write(buff);
      break;

    default:
      break;
  }
}
