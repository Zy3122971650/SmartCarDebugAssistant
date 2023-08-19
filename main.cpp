#include "smartcarvisual.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartCarVisual w;
    w.show();
    return a.exec();
}
