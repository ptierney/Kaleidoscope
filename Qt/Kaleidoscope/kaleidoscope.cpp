#include "kaleidoscope.h"
#include "ui_kaleidoscope.h"

namespace Kaleidoscope {

Kaleidoscope::Kaleidoscope(QApplication* ap, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kaleidoscope) {
    ui->setupUi(this);
    d = new Kal::Device(ap, this);
    createActions();
    createMenus();
}

Kaleidoscope::~Kaleidoscope() {
    delete d;
    delete ui;
}

QSize  Kaleidoscope::sizeHint() const {
        return QSize(DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT);
}

void Kaleidoscope::init() {
    d->init();
}

void Kaleidoscope::createActions() {
	//initAct = new QAction(tr("&Init"),this);
	//connect(initAct, SIGNAL(triggered()), this, SLOT(init()));
}

void Kaleidoscope::createMenus() {
    /*
    gridsMenu = menuBar()->addMenu(tr("&Grids"));
    gridsMenu->addAction(initAct);
    */
}

} // end namespace Kaleidoscope
