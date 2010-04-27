#include <QtGui/QApplication>
#include <QtOpenGL>
#include "kaleidoscope.h"

int main(int argc, char *argv[])
{
    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);
    QApplication a(argc, argv);
    Kaleidoscope::Kaleidoscope w(&a);
    //w.show();
    //w.init();
    return a.exec();
}
