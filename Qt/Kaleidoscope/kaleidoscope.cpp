#include "kaleidoscope.h"
#include "ui_kaleidoscope.h"

Kaleidoscope::Kaleidoscope(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kaleidoscope)
{
    ui->setupUi(this);
}

Kaleidoscope::~Kaleidoscope()
{
    delete ui;
}
