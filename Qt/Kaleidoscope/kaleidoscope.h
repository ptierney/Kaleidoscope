#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

#include <QtGui/QMainWindow>
#include <kaleidoscope/device.h>

namespace Ui
{
    class Kaleidoscope;
}

namespace Kaleidoscope {

class Kaleidoscope : public QMainWindow {
    Q_OBJECT

public:
    Kaleidoscope(QApplication* ap, QWidget *parent = 0);
    ~Kaleidoscope();

    QSize sizeHint() const;

private:
    Ui::Kaleidoscope *ui;
    Kal::Device* d;
};

} // end namespace Kaleidoscope

#endif // KALEIDOSCOPE_H
