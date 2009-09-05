#include "kaleidoscope.h"
#include "ui_kaleidoscope.h"

Kaleidoscope::Kaleidoscope(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kaleidoscope)
{
    ui->setupUi(this);
    d = new Kal::Device(this);
}

Kaleidoscope::~Kaleidoscope()
{
    delete d;
    delete ui;
}

QSize  Kaleidoscope::sizeHint() const {
	return QSize(600,375);
}
