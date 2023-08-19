#ifndef SMARTCARVISUAL_H
#define SMARTCARVISUAL_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class SmartCarVisual; }
QT_END_NAMESPACE

class SmartCarVisual : public QMainWindow
{
    Q_OBJECT

public:
    SmartCarVisual(QWidget *parent = nullptr);
    ~SmartCarVisual();



private:
    Ui::SmartCarVisual *ui;

};
#endif // SMARTCARVISUAL_H
