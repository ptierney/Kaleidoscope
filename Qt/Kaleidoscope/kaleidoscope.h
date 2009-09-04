#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class Kaleidoscope;
}

class Kaleidoscope : public QMainWindow
{
    Q_OBJECT

public:
    Kaleidoscope(QWidget *parent = 0);
    ~Kaleidoscope();

private:
    Ui::Kaleidoscope *ui;
};

#endif // KALEIDOSCOPE_H
