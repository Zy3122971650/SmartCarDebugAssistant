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

  void updateRecieveBrower(const QByteArray &data);

  void updateViewDraw(void);

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_imgSlider_valueChanged(int value);

  void on_tabWidget_currentChanged(int index);

  void on_ImgCurrent_editingFinished();

  void on_horizontalScrollBar_valueChanged(int value);

  void point_draw_update(bool current);

  void on_pointData1_stateChanged(int arg1);

  void on_pointData7_stateChanged(int arg1);

  void on_pointData2_stateChanged(int arg1);

  void on_pointData3_stateChanged(int arg1);

  void on_pointData4_stateChanged(int arg1);

  void on_pointData5_stateChanged(int arg1);

  void on_pointData6_stateChanged(int arg1);

  void on_pointData8_stateChanged(int arg1);

  void on_pointData9_stateChanged(int arg1);

  void on_pushButton_5_clicked();
  void update_pointScroll(void);

  void on_pushButton_7_clicked();

  private:
  Ui::SmartCarVisual *ui;
  Communication::communication_hal *communication;

  void update_img(bool current);
  void clean_data(void);
  void update_data_label(void);
};
#endif  // SMARTCARVISUAL_H
