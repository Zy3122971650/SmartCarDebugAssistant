#include "smartcarvisual.h"

#include <QSerialPortInfo>

#include "./ui_smartcarvisual.h"

SmartCarVisual::SmartCarVisual(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmartCarVisual) {
  ui->setupUi(this);

  this->communication = new Communication::communication_hal();
  connect(this->communication,
          &Communication::communication_hal::communicationRecieve, this,
          &SmartCarVisual::updateRecieveBrower);
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

    auto buff = QByteArray();
    if (this->ui->communicationSendHex->isChecked()) {
      QString n = QString();
      // 读取所有字符，找到`0x`全部去掉，找到a-f以外的字符全部当错误处理，剩下的两个两个组成一个16进制数
      for (int i = 0; i < s.length(); i++) {
        QChar c = s[i].toLower();
        if ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9')) {
          n.append(c);
        }
      }

      if (n.length() % 2 != 0) {
        // 不是双数说明格式有问题
        return;
      }

      for (int i = 0; i < n.length(); i += 2) {
        QString byteString = n.mid(i, 2);
        bool ok;
        char byteValue = byteString.toInt(&ok, 16);
        if (ok) {
          buff.append(byteValue);
        } else {
          // 错误处理，无法转换为有效的十六进制值
          return;
        }
      }
    } else {
      buff = s.toLocal8Bit();
    }
    this->communication->write(buff);
    this->ui->communicationSendText->clear();
    QString n = QString();
    if (this->ui->communicationSendHex->isChecked()) {
      for (int i = 0; i < buff.size(); i++) {
        n.append(QString("0x%1").arg(static_cast<unsigned char>(buff[i]), 2, 16,
                                     QChar('0')));

        if (i < buff.size() - 1) {
          n.append(" ");
        }
      }
    } else {
      for (int i = 0; i < buff.size(); i++) {
        n.append(QChar(static_cast<unsigned char>(buff[i])));

        if (i < buff.size() - 1) {
          n.append(" ");
        }
      }
    }
    this->ui->communicationSendBrower->append(n);
    this->ui->sendCount->setText(
        QString::number(this->ui->sendCount->text().toInt() + buff.length()));
  }
}

void SmartCarVisual::updateRecieveBrower(const QByteArray &data) {
  QString n = QString();

  if (this->ui->communicationSendHex->isChecked()) {
    for (int i = 0; i < data.size(); i++) {
      n.append(QString("0x%1").arg(static_cast<unsigned char>(data[i]), 2, 16,
                                   QChar('0')));

      if (i < data.size() - 1) {
        n.append(" ");
      }
    }
  } else {
    for (int i = 0; i < data.size(); i++) {
      n.append(QChar(static_cast<unsigned char>(data[i])));

      if (i < data.size() - 1) {
        n.append(" ");
      }
    }
  }
  this->ui->communicationReciveBrower->append(n);
  this->ui->reciveCount->setText(
      QString::number(this->ui->sendCount->text().toInt() + data.length()));
}

void SmartCarVisual::on_pushButton_3_clicked() {
  this->ui->communicationSendBrower->clear();
}

void SmartCarVisual::on_pushButton_4_clicked() {
  this->ui->communicationReciveBrower->clear();
}
