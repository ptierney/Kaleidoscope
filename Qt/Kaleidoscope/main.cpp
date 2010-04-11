#include <QtGui/QApplication>
#include "kaleidoscope.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kaleidoscope::Kaleidoscope w(&a);
    //w.show();
    //w.init();
    return a.exec();
}
