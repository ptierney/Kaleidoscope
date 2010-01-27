#include <QtGui/QApplication>
#include "kaleidoscope.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kal::Kaleidoscope w(&a);
    //w.show();
    //w.init();
    return a.exec();
}
