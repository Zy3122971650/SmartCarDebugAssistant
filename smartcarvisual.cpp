#include "smartcarvisual.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "./ui_smartcarvisual.h"

SmartCarVisual::SmartCarVisual(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmartCarVisual) {
  ui->setupUi(this);
}

SmartCarVisual::~SmartCarVisual() { delete ui; }
