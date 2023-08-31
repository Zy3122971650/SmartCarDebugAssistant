#pragma once
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
#include <communication.hpp>
using namespace Communication;
class CustomGraphicsView : public QGraphicsView {
  Q_OBJECT
 signals:
  void showPointValueLable(void);

 public:
  CustomGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent) {}
  qreal x_factor = 10.0;
  qreal y_factor = 10.0;
  int scoll_max = 0;
  qreal view_x = 0;
  qreal view_y = 0;
  QVector<struct Data> *data;
  QPoint initialPosition;
  bool isMousePressed = false;
  bool isCtrlPressed = false;
  qreal y_max_global = 0;
  qreal y_min_global = 0;

  bool inital = false;
  // 配置可见性
  QGraphicsPathItem *waveform1 = nullptr;
  QGraphicsPathItem *waveform2 = nullptr;
  QGraphicsPathItem *waveform3 = nullptr;
  QGraphicsPathItem *waveform4 = nullptr;
  QGraphicsPathItem *waveform5 = nullptr;
  QGraphicsPathItem *waveform6 = nullptr;
  QGraphicsPathItem *waveform7 = nullptr;
  QGraphicsPathItem *waveform8 = nullptr;
  QGraphicsPathItem *waveform9 = nullptr;
  bool visual1 = true;
  bool visual2 = true;
  bool visual3 = true;
  bool visual4 = true;
  bool visual5 = true;
  bool visual6 = true;
  bool visual7 = true;
  bool visual8 = true;
  bool visual9 = true;

  // 应该显示的数据
  uint32_t point_show_id = 0;

  void update_points(void);
  void update_view_port(void);

 protected:
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      // 当鼠标左键按下时执行的操作
      // 记录初始位置
      initialPosition = event->pos();
      isMousePressed = true;
    }

    QWidget::mousePressEvent(event);
  }
  void keyPressEvent(QKeyEvent *event) override {
    if (event->key() == Qt::Key_Control) {
      this->isCtrlPressed = true;
    }
  }

  void keyReleaseEvent(QKeyEvent *event) override {
    if (event->key() == Qt::Key_Control) {
      this->isCtrlPressed = false;
    }
  }
  void mouseMoveEvent(QMouseEvent *event) override {
    if (isMousePressed) {
      // 当鼠标左键按住并移动时执行的操作
      QPoint currentPosition = event->pos();
      QPoint delta = currentPosition - initialPosition;
      // qDebug() << delta;
      qDebug() << this->view_x;
      qDebug() << this->view_y;

      this->view_x -= delta.x();
      this->view_y -= delta.y();
      qDebug() << this->view_x;
      qDebug() << this->view_y;

      initialPosition = currentPosition;
      qreal x = this->view_x;
      qreal y = this->view_y;
      this->update_points();
      this->view_x = x;
      this->view_y = y;
      this->update_view_port();
    } else {
      // 获取位置，计算对应的实际x轴位置
      qreal temp = (this->view_x + this->pos().x()) / this->x_factor;
      if (temp < 0) {
        temp = 0;
      }
      this->point_show_id = temp;
      emit showPointValueLable();
    }

    QWidget::mouseMoveEvent(event);
  }

  void mouseReleaseEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      // 当鼠标左键释放时执行的操作
      isMousePressed = false;
    }

    QWidget::mouseReleaseEvent(event);
  }
};