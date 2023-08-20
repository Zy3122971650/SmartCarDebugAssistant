#ifndef SMARTCARVISUAL_H
#define SMARTCARVISUAL_H

#include <QMainWindow>

#include "communication.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class SmartCarVisual;
}
QT_END_NAMESPACE

class SmartCarVisual : public QMainWindow {
  Q_OBJECT

 public:
  SmartCarVisual(QWidget *parent = nullptr);
  ~SmartCarVisual();

 private slots:
  void on_serialButton_clicked();

  void on_socketButton_clicked();

  void on_SerialDeviceFlash_clicked();

  void on_communicationSend_clicked();

  private:
  Ui::SmartCarVisual *ui;
  Communication::communication_hal *communication;
};
#endif  // SMARTCARVISUAL_H
