#include <QtGui/QApplication>
#include "kaleidoscope.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kaleidoscope w(a);
    w.show();
    return a.exec();
}