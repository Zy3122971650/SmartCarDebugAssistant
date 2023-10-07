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
      port->setReadBufferSize(10485760);
      if (!port->open(QIODeviceBase::ReadWrite)) {
        delete port;
        return false;
      }
      this->connect_type = t;
      this->serial_port = port;
      QObject::connect(this->serial_port, SIGNAL(readyRead()), this,
                       SLOT(callbackReadyDataSlot()));
    } break;
    case ConnectType::Socket: {
      QTcpSocket *socket = new QTcpSocket();
      socket->connectToHost(path, port_or_baud.toInt());
      if (!socket->isValid()) {
        delete socket;
        return false;
      }
      this->connect_type = t;
      this->socket_port = socket;
      QObject::connect(this->socket_port, SIGNAL(readyRead()), this,
                       SLOT(callbackReadyDataSlot()));

    } break;
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
    case ConnectType::Socket:
      this->socket_port->close();
      delete this->socket_port;
      this->socket_port = nullptr;
    default:
      break;
  }
}

void communication_hal::callbackReadyDataSlot() {
  // TODO: 漏了socket分支
  // qDebug() << "数据准备好了";
  QByteArray data;
  switch (this->connect_type) {
    case ConnectType::Serial:
      data = this->serial_port->readAll();

      break;
    case ConnectType::Socket: {
      data = this->socket_port->readAll();
    }
    default:
      break;
  }

  // qDebug() << "数据是：" << data;
  if (decode(data, &this->data)) {
    emit frameDecodeFinished();
  }
  // TODO: 设计一个信号，把数据传递到显示窗口
  emit communicationRecieve(data);

  // 发送信号到绘制函数，让绘制函数自己检查有无新数据要绘制
}
communication_hal::communication_hal() {
  this->socket_port = nullptr;
  this->serial_port = nullptr;
  this->connect_type = ConnectType::Serial;
  struct Data data;
  this->data = data;
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
