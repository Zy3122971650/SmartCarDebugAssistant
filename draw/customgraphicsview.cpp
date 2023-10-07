#include "customgraphicsview.h"

#include "QString"
void CustomGraphicsView::wheelEvent(QWheelEvent *event) {
  qreal inc = 0.5;
  qreal last_factor = 0;
  QPointF position = event->position();
  if (!isCtrlPressed) {
    if (x_factor < 1.0) {
      inc = 0.1;
    }
    last_factor = x_factor;
    if (event->angleDelta().y() > 0 || event->pixelDelta().y() < 0) {
      x_factor += inc;
    } else {
      x_factor -= inc;
    }
    if (x_factor < 0.1) {
      x_factor = 0.1;
      return;
    }
    view_x = x_factor * ((view_x + position.x()) / last_factor) - position.x();

  } else {
    if (y_factor < 1.0) {
      inc = 0.1;
    }
    last_factor = y_factor;
    if (event->angleDelta().y() > 0 || event->pixelDelta().y() > 0) {
      y_factor += inc;
    } else {
      y_factor -= inc;
    }
    if (y_factor < 0.1) {
      y_factor = 0.1;
      return;
    }
    view_y = y_factor / last_factor *
                 ((qreal)this->height() - position.y() + view_y) +
             position.y() - (qreal)this->height();
  }

  emit updateShowPointScroll();
  event->ignore();
}

void CustomGraphicsView::update_points() {
  blockSignals(true);
  if (this->scene()) {
    delete this->scene();
  }
  QGraphicsScene *scene = new QGraphicsScene();

  // 计算所要绘制的点的位置
  uint32_t id_min = qMax((this->view_x / this->x_factor) - 2, (double)0);

  uint32_t id_max =
      qMin((this->view_x + (qreal)this->width()) / this->x_factor + 2,
           (double)this->data->points.length());

  if (id_min == id_max) {
    delete scene;
    return;
  }
  qreal offset = this->view_x;

  QPen pen1(Qt::blue);
  QPainterPath Path1;
  Path1.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(0) * this->y_factor);
  for (uint32_t i = id_min; i < id_max; ++i) {
    Path1.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(0) * this->y_factor);
  }
  waveform1 = scene->addPath(Path1, pen1);
  waveform1->setVisible(visual1);

  QPen pen2(Qt::red);
  QPainterPath Path2;
  Path2.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(1) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path2.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(1) * this->y_factor);
  }
  waveform2 = scene->addPath(Path2, pen2);
  waveform2->setVisible(visual2);

  QPen pen3(Qt::yellow);
  QPainterPath Path3;
  Path3.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(2) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path3.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(2) * this->y_factor);
  }
  waveform3 = scene->addPath(Path3, pen3);
  waveform3->setVisible(visual3);

  QPen pen4(Qt::darkCyan);
  QPainterPath Path4;
  Path4.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(3) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path4.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(3) * this->y_factor);
  }
  waveform4 = scene->addPath(Path4, pen4);
  waveform4->setVisible(visual4);

  QPen pen5(Qt::darkGreen);
  QPainterPath Path5;
  Path5.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(4) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path5.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(4) * this->y_factor);
  }
  waveform5 = scene->addPath(Path5, pen5);
  waveform5->setVisible(visual5);

  QPen pen6(Qt::darkBlue);
  QPainterPath Path6;
  Path6.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(5) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path6.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(5) * this->y_factor);
  }
  waveform6 = scene->addPath(Path6, pen6);
  waveform6->setVisible(visual6);

  QPen pen7(Qt::darkRed);
  QPainterPath Path7;
  Path7.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(6) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path7.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(6) * this->y_factor);
  }
  waveform7 = scene->addPath(Path7, pen7);
  waveform7->setVisible(visual7);

  QPen pen8(Qt::darkYellow);
  QPainterPath Path8;
  Path8.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(7) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path8.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(7) * this->y_factor);
  }
  waveform8 = scene->addPath(Path8, pen8);
  waveform8->setVisible(visual8);

  QPen pen9(Qt::green);
  QPainterPath Path9;
  Path9.moveTo((qreal)id_min * this->x_factor - offset,
               -this->data->points.at(id_min).at(8) * this->y_factor);
  for (uint32_t i = id_min + 1; i < id_max; ++i) {
    Path9.lineTo((qreal)i * this->x_factor - offset,
                 -this->data->points.at(i).at(8) * this->y_factor);
  }
  waveform9 = scene->addPath(Path9, pen9);
  waveform9->setVisible(visual9);

  this->setRenderHint(QPainter::Antialiasing, true);
  this->setScene(scene);
  this->scene()->setSceneRect(0, this->view_y, this->width(), this->height());
  
  // 绘制坐标轴
  // QPen axisPen(Qt::transparent, 2);
  // QGraphicsLineItem *axisX = scene->addLine(
  //     0, 0, (qreal)this->data->length() * this->x_factor, 0, axisPen);
  // QGraphicsLineItem *axisY =
  //     scene->addLine(this->view_x + 5, -y_min_global * this->y_factor,
  //                    this->view_x + 5, -y_max_global * this->y_factor,
  //                    axisPen);
  // if (y_max_global != y_min_global) {
  //   QGraphicsTextItem *tickLabel =
  //       scene->addText(QString::number(y_max_global));
  //   tickLabel->setPos(this->view_x + 5,
  //                     -y_max_global * this->y_factor);  // 调整刻度值位置
  //   QGraphicsTextItem *tickLabel1 =
  //       scene->addText(QString::number(y_min_global));
  //   tickLabel1->setPos(this->view_x + 5,
  //                      -y_min_global * this->y_factor);  // 调整刻度值位置
  // } else {
  //   QGraphicsTextItem *tickLabel =
  //       scene->addText(QString::number(y_max_global));
  //   ->setPos(this->view_x + 5,
  //            -y_max_global * this->y_factor);  // 调整刻度值位置
  // }

  // 配置GUI
  if ((qreal)this->data->points.length() * this->x_factor - scene->width() >=
      0) {
    this->scoll_max =
        (qreal)this->data->points.length() * this->x_factor - scene->width();
  } else {
    this->scoll_max = 0;
  }
  if (!inital) {
    this->view_x = 0;
    this->view_y = -this->height() / 2;
    this->inital = true;
  }
  blockSignals(false);
}

void CustomGraphicsView::update_view_port() {
  // this->scene()->setSceneRect(this->view_x, this->view_y, this->width(),
  //                             this->height());
  // this->update_points();
}
