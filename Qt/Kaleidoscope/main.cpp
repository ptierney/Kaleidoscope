#include <QtGui/QApplication>
#include "kaleidoscope.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kaleidoscope w;
    w.show();
    return a.exec();
}
