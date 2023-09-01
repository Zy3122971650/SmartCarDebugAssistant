#include "customgraphicsview.h"

#include "QString"
void CustomGraphicsView::wheelEvent(QWheelEvent *event) {
  qreal factor = 1;
  qreal inc = 0.5;
  qreal old_x = 0;
  qreal old_y = 0;
  old_x = this->view_x;
  old_y = this->view_y;

  if (!isCtrlPressed) {
    if (x_factor < 1.0) {
      inc = 0.1;
    }
    if (event->pixelDelta().y() > 0) {
      factor += (inc) / x_factor;
      x_factor += inc;
    } else {
      factor += (-inc) / x_factor;
      x_factor -= inc;
    }
    if (x_factor < 0.1) {
      x_factor = 0.1;
      return;
    }
  } else {
    if (y_factor < 1.0) {
      inc = 0.1;
    }
    old_y = this->view_y;
    if (event->pixelDelta().y() > 0) {
      factor += (inc) / y_factor;
      y_factor += inc;
    } else {
      factor += (-inc) / y_factor;
      y_factor -= inc;
    }
    if (y_factor < 0.1) {
      y_factor = 0.1;
      return;
    }
  }
  // 重新计算view port位置
  if (!isCtrlPressed) {
    this->view_x =
        (event->position().x() + old_x) * factor - event->position().x();
    this->view_y = old_y;
  } else {
    this->view_y =
        (event->position().y() + old_y) * factor - event->position().y();
    this->view_x = old_x;
  }
  this->update_points();
  this->update_view_port();

  event->ignore();
}

void CustomGraphicsView::update_points() {
  if (this->scene()) {
    delete this->scene();
  }
  QGraphicsScene *scene = new QGraphicsScene();
  QPen pen1(Qt::blue);
  QPainterPath Path1;
  Path1.moveTo(0, 0);

  QPen pen2(Qt::red);
  QPainterPath Path2;
  Path2.moveTo(0, 0);

  QPen pen3(Qt::yellow);
  QPainterPath Path3;
  Path3.moveTo(0, 0);

  QPen pen4(Qt::green);
  QPainterPath Path4;
  Path4.moveTo(0, 0);

  QPen pen5(Qt::cyan);
  QPainterPath Path5;
  Path5.moveTo(0, 0);

  QPen pen6(Qt::darkYellow);
  QPainterPath Path6;
  Path6.moveTo(0, 0);

  QPen pen7(Qt::darkGray);
  QPainterPath Path7;
  Path7.moveTo(0, 0);

  QPen pen8(Qt::darkBlue);
  QPainterPath Path8;
  Path8.moveTo(0, 0);

  QPen pen9(Qt::darkRed);
  QPainterPath Path9;
  Path9.moveTo(0, 0);
  for (int i = 0; i < this->data->length(); ++i) {
    if (this->data->at(i).points == nullptr) {
      continue;
    }
    Path1.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(0) * this->y_factor);
    Path2.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(1) * this->y_factor);
    Path3.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(2) * this->y_factor);
    Path4.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(3) * this->y_factor);
    Path5.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(4) * this->y_factor);
    Path6.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(5) * this->y_factor);
    Path7.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(6) * this->y_factor);
    Path8.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(7) * this->y_factor);
    Path9.lineTo(i * this->x_factor,
                 -this->data->at(i).points->at(8) * this->y_factor);
    QVector<float> temp = *this->data->at(i).points;
    std::sort(temp.begin(), temp.end());
    if (temp.last() > y_max_global) {
      y_max_global = temp.last();
    }
    if (temp.first() < y_min_global) {
      y_min_global = temp.first();
    }
    if (i % 10 == 0) {
      QGraphicsTextItem *tickLabel = scene->addText(QString::number(i));
      tickLabel->setPos(i * this->x_factor, 5);  // 调整刻度值位置
    }
  }

  // for (int i = 0; i <= numTicks; ++i) {
  //   qreal x = i * tickSpacing;
  //   scene->addLine(x, viewRect.bottom(), x, viewRect.bottom() - 10,
  //   axisPen);

  //   QGraphicsTextItem *tickLabel = scene->addText(QString::number(x));
  //   tickLabel->setPos(x - 10, viewRect.bottom() + 5);  // 调整刻度值位置
  // }
  // 绘制
  // 加载scene

  waveform1 = scene->addPath(Path1, pen1);
  waveform2 = scene->addPath(Path2, pen2);
  waveform3 = scene->addPath(Path3, pen3);
  waveform4 = scene->addPath(Path4, pen4);
  waveform5 = scene->addPath(Path5, pen5);
  waveform6 = scene->addPath(Path6, pen6);
  waveform7 = scene->addPath(Path7, pen7);
  waveform8 = scene->addPath(Path8, pen8);
  waveform9 = scene->addPath(Path9, pen9);
  waveform1->setVisible(visual1);
  waveform2->setVisible(visual2);
  waveform3->setVisible(visual3);
  waveform4->setVisible(visual4);
  waveform5->setVisible(visual5);
  waveform6->setVisible(visual6);
  waveform7->setVisible(visual7);
  waveform8->setVisible(visual8);
  waveform9->setVisible(visual9);
  // this->fitInView(waveform, Qt::KeepAspectRatio);
  this->setRenderHint(QPainter::Antialiasing, true);
  this->setScene(scene);

  // 绘制坐标轴
  QPen axisPen(Qt::transparent, 2);
  QGraphicsLineItem *axisX = scene->addLine(
      0, 0, (qreal)this->data->length() * this->x_factor, 0, axisPen);
  QGraphicsLineItem *axisY =
      scene->addLine(this->view_x + 5, -y_min_global * this->y_factor,
                     this->view_x + 5, -y_max_global * this->y_factor, axisPen);
  if (y_max_global != y_min_global) {
    QGraphicsTextItem *tickLabel =
        scene->addText(QString::number(y_max_global));
    tickLabel->setPos(this->view_x + 5,
                      -y_max_global * this->y_factor);  // 调整刻度值位置
    QGraphicsTextItem *tickLabel1 =
        scene->addText(QString::number(y_min_global));
    tickLabel1->setPos(this->view_x + 5,
                       -y_min_global * this->y_factor);  // 调整刻度值位置
  } else {
    QGraphicsTextItem *tickLabel =
        scene->addText(QString::number(y_max_global));
    tickLabel->setPos(this->view_x + 5,
                      -y_max_global * this->y_factor);  // 调整刻度值位置
  }

  // 配置GUI
  this->scoll_max = this->data->length() * ceil(this->x_factor);
  if (!inital) {
    this->view_x = 0;
    this->view_y = -this->height() / 2;
    this->inital = true;
  }
  update_view_port();
}

void CustomGraphicsView::update_view_port() {
  this->scene()->setSceneRect(this->view_x, this->view_y, this->width(),
                              this->height());
}
