#include "smartcarvisual.h"

#include <QSerialPortInfo>

#include "./ui_smartcarvisual.h"

SmartCarVisual::SmartCarVisual(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmartCarVisual) {
  ui->setupUi(this);

  this->communication = new Communication::communication_hal();
}

SmartCarVisual::~SmartCarVisual() {
  delete ui;
  delete communication;
}

void SmartCarVisual::on_serialButton_clicked() {
  if (this->ui->seriaPortListBox->isEnabled() &&
      this->communication->socket_port == nullptr) {
    // 说明还没有锁定,尝试打开串口
    if (this->communication->open(
            Communication::ConnectType::Serial,
            this->ui->seriaPortListBox->currentText().split('(')[0],
            this->ui->baudRateEdit->text())) {
      // 成功打开串口，锁定相关ui
      this->ui->baudRateEdit->setEnabled(false);
      this->ui->seriaPortListBox->setEnabled(false);
      this->ui->SerialDeviceFlash->setEnabled(false);
      this->ui->serialButton->setText("关闭串口");
    } else {
      qDebug() << "打开串口失败,检查连接和波特率";
    }
  } else {
    this->communication->close();
    this->ui->baudRateEdit->setEnabled(true);
    this->ui->seriaPortListBox->setEnabled(true);
    this->ui->SerialDeviceFlash->setEnabled(true);
    this->ui->serialButton->setText("打开串口");
  }
}

void SmartCarVisual::on_socketButton_clicked() {
  if (this->ui->IpAddr->isEnabled() &&
      this->communication->serial_port == nullptr) {
    // 说明还没有锁定,尝试打开串口
    if (this->communication->open(Communication::ConnectType::Socket,
                                  this->ui->IpAddr->text(),
                                  this->ui->PortAddr->text())) {
      // 成功打开串口，锁定相关ui
      this->ui->IpAddr->setEnabled(false);
      this->ui->PortAddr->setEnabled(false);
      this->ui->socketButton->setText("关闭连接");
    } else {
      qDebug() << "打开socket失败,检查连接和波特率";
    }
  } else {
    this->communication->close();
    this->ui->IpAddr->setEnabled(true);
    this->ui->PortAddr->setEnabled(true);
    this->ui->socketButton->setText("打开连接");
  }
}

void SmartCarVisual::on_SerialDeviceFlash_clicked() {
  QStringList list;
  foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
    list << info.portName() + "(" + info.serialNumber() + ")";
  }
  std::sort(list.begin(), list.end());
  this->ui->seriaPortListBox->clear();
  this->ui->seriaPortListBox->addItems(list);
}

void SmartCarVisual::on_communicationSend_clicked() {
  // 判断文本框有无字符以及端口是否打开，再发送字符
  if (this->communication->serial_port || this->communication->socket_port) {
    QString s = this->ui->communicationSendText->toPlainText();
    if (s.isEmpty()) {
      return;
    }
    auto buff = s.toLocal8Bit();
    this->communication->write(buff);
  }
}
