#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

#include <QtGui/QMainWindow>
#include <kaleidoscope/device>

namespace Ui
{
    class Kaleidoscope;
}

class Kaleidoscope : public QMainWindow {
    Q_OBJECT

public:
    Kaleidoscope(QWidget *parent = 0);
    ~Kaleidoscope();

    QSize sizeHint() const;

private:
    Ui::Kaleidoscope *ui;
    Kal::Device* d;
};

#endif // KALEIDOSCOPE_H
