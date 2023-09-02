#include "smartcarvisual.h"

#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QWheelEvent>

#include "./ui_smartcarvisual.h"

SmartCarVisual::SmartCarVisual(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmartCarVisual) {
  ui->setupUi(this);
  this->communication = new Communication::communication_hal();

  // 图像部分GUI初始化
  this->ui->imgTotal->setEnabled(false);
  this->ui->imgTotal->setText(QString::number(0));
  this->ui->imgSlider->setMaximum(0);
  this->ui->imgSlider->setPageStep(1);
  // 数据显示部分GUI初始化
  this->ui->pointsDrawView->setHorizontalScrollBarPolicy(
      Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
  this->ui->pointsDrawView->setVerticalScrollBarPolicy(
      Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
  this->ui->pointsDrawView->data = &this->communication->data;
  this->ui->pointData1->setChecked(true);
  this->ui->pointData2->setChecked(true);
  this->ui->pointData3->setChecked(true);
  this->ui->pointData4->setChecked(true);
  this->ui->pointData5->setChecked(true);
  this->ui->pointData6->setChecked(true);
  this->ui->pointData7->setChecked(true);
  this->ui->pointData8->setChecked(true);
  this->ui->pointData9->setChecked(true);

  connect(this->communication,
          &Communication::communication_hal::communicationRecieve, this,
          &SmartCarVisual::updateRecieveBrower);
  connect(this->communication,
          &Communication::communication_hal::frameDecodeFinished, this,
          &SmartCarVisual::updateViewDraw);
  connect(this->ui->pointsDrawView, &CustomGraphicsView::showPointValueLable,
          this, &SmartCarVisual::update_data_label);
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
      QString::number(this->ui->reciveCount->text().toInt() + data.length()));
}

void SmartCarVisual::on_pushButton_3_clicked() {
  this->ui->communicationSendBrower->clear();
}

void SmartCarVisual::on_pushButton_4_clicked() {
  this->ui->communicationReciveBrower->clear();
}

void SmartCarVisual::updateViewDraw() {
  this->update_img(true);
  this->ui->pointsDrawView->update_points();
}

void SmartCarVisual::update_img(bool current) {
  // 绘制的时候要阻断信号传播，当成一个原子操作
  // 根据data中的数据更新总图片个数
  auto total = this->communication->data.length();
  if (!total) {
    return;
  }
  this->ui->imgTotal->setText(QString::number(total));
  // 根据data中的数据更新滑动条
  this->ui->imgSlider->setMaximum(total - 1);

  if (current) this->ui->imgSlider->setValue(total - 1);
  // 绘制对应数据到图框(从滑动条得到index,滑动条只能被串口来的回调更新)
  auto index = this->ui->imgSlider->value();
  auto datas = this->communication->data[index].img;
  // 解析帧
  foreach (auto frame, datas) {
    // 绘制图号
    auto draw_index = frame.draw_id;
    // 0 原图混合
    switch (draw_index) {
      case 0:
        // 清除上一刻的场景
        if (!this->ui->graphicsView_0->scene()) {
          delete this->ui->graphicsView_0->scene();
        }
        {
          QGraphicsScene *scene = new QGraphicsScene();
          scene->addItem(frame.item);
          this->ui->graphicsView_0->setScene(scene);
        }
        break;
    }
  }
}
void SmartCarVisual::on_imgSlider_valueChanged(int value) {
  blockSignals(true);
  this->ui->ImgCurrent->setText(QString::number(value + 1));
  update_img(false);
  blockSignals(false);
}

void SmartCarVisual::on_tabWidget_currentChanged(int index) {
  if (index == 3) {
    update_img(false);
  } else if (index == 1) {
    this->ui->pointsDrawView->update_points();
    this->ui->pointsDrawView->view_x = 0;
    this->ui->pointsDrawView->view_y = -this->ui->pointsDrawView->height() / 2;
  }
}

void SmartCarVisual::on_ImgCurrent_editingFinished() {
  this->ui->imgSlider->setValue(this->ui->ImgCurrent->text().toInt() - 1);
}

void SmartCarVisual::on_horizontalScrollBar_valueChanged(int value) {
  this->ui->horizontalScrollBar->setMaximum(
      this->ui->pointsDrawView->scoll_max);

  //
  this->ui->pointsDrawView->view_x = this->ui->horizontalScrollBar->value();
  this->ui->pointsDrawView->update_view_port();
}

void SmartCarVisual::point_draw_update() {
  this->ui->pointsDrawView->update_points();
}

// 波形绘制GUI部分响应
void SmartCarVisual::on_pointData1_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual1 = arg1;
  auto wave = this->ui->pointsDrawView->waveform1;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual1);
}

void SmartCarVisual::on_pointData2_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual2 = arg1;

  auto wave = this->ui->pointsDrawView->waveform2;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual2);
}

void SmartCarVisual::on_pointData3_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual3 = arg1;

  auto wave = this->ui->pointsDrawView->waveform3;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual3);
}

void SmartCarVisual::on_pointData4_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual4 = arg1;

  auto wave = this->ui->pointsDrawView->waveform4;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual4);
}

void SmartCarVisual::on_pointData5_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual5 = arg1;
  auto wave = this->ui->pointsDrawView->waveform5;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual5);
}

void SmartCarVisual::on_pointData6_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual6 = arg1;
  auto wave = this->ui->pointsDrawView->waveform6;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual6);
}
void SmartCarVisual::on_pointData7_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual7 = arg1;
  auto wave = this->ui->pointsDrawView->waveform7;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual7);
}

void SmartCarVisual::on_pointData8_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual8 = arg1;
  auto wave = this->ui->pointsDrawView->waveform8;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual8);
}

void SmartCarVisual::on_pointData9_stateChanged(int arg1) {
  this->ui->pointsDrawView->visual9 = arg1;
  auto wave = this->ui->pointsDrawView->waveform9;
  if (!wave) {
    return;
  }
  wave->setVisible(this->ui->pointsDrawView->visual9);
}

void SmartCarVisual::on_pushButton_5_clicked() { clean_data(); }

void SmartCarVisual::clean_data() {
  // 清除所有数据
  // 遍历所有数据
  foreach (auto frame, this->communication->data) {
    // 清理图片
    foreach (auto img, frame.img) {
      delete img.item;
    }
    // 清理全部波形绘制point
    delete frame.points;
  }
  this->communication->data.clear();
  updateViewDraw();
}

void SmartCarVisual::update_data_label(void) {
  if (this->ui->pointsDrawView->point_show_id <
          this->ui->pointsDrawView->data->length() &&
      this->ui->pointsDrawView->data
              ->at(this->ui->pointsDrawView->point_show_id)
              .points != nullptr) {
    this->ui->pointShow1->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(0)));
    this->ui->pointShow2->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(1)));
    this->ui->pointShow3->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(2)));
    this->ui->pointShow4->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(3)));
    this->ui->pointShow5->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(4)));
    this->ui->pointShow6->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(5)));
    this->ui->pointShow7->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(6)));
    this->ui->pointShow8->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(7)));
    this->ui->pointShow9->setText(
        QString::number(this->ui->pointsDrawView->data
                            ->at(this->ui->pointsDrawView->point_show_id)
                            .points->at(8)));
  }
}
