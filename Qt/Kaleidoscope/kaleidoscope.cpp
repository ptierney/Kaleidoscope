#include "kaleidoscope.h"
#include "ui_kaleidoscope.h"

namespace Kaleidoscope {

Kaleidoscope::Kaleidoscope(QApplication* ap, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kaleidoscope) {
    ui->setupUi(this);
    d = new Kal::Device(ap, this);
}

Kaleidoscope::~Kaleidoscope() {
    delete d;
    delete ui;
}

QSize  Kaleidoscope::sizeHint() const {
	return QSize(600,375);
}

} // end namespace Kaleidoscope
