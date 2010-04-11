#include "kaleidoscope.h"
#include "ui_kaleidoscope.h"

#include <kaleidoscope/device.h>

namespace Kaleidoscope {

Kaleidoscope::Kaleidoscope(QApplication* ap, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kaleidoscope) {
    ui->setupUi(this);
    d = new Device(ap, this);
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

void Kaleidoscope::createUserInputWindow(){
  d->createUserInputWindow();
}

void Kaleidoscope::createAddUserWindow(){
  d->createAddUserWindow();
}

void Kaleidoscope::createNewChatWindow(){

}

void Kaleidoscope::createActions() {
	//initAct = new QAction(tr("&Init"),this);
	//connect(initAct, SIGNAL(triggered()), this, SLOT(init()));
  set_name_act_ = new QAction(tr("Set name..."),this);
  connect(set_name_act_, SIGNAL(triggered()),
          this, SLOT(createUserInputWindow()));

  register_username_ = new QAction(tr("&Add account..."), this);
  connect(register_username_, SIGNAL(triggered()),
          this, SLOT(createAddUserWindow()));

  new_chat_ = new QAction(tr("&New chat..."), this);
  connect(new_chat_, SIGNAL(triggered()),
          this, SLOT(createNewChatWindow()));
}

void Kaleidoscope::createMenus() {
  //gridsMenu = menuBar()->addMenu(tr("&Grids"));
  //gridsMenu->addAction(initAct);


  start_menu_ = menuBar()->addMenu(tr("Start here"));
  start_menu_->addAction(set_name_act_);
  start_menu_->addSeparator();
  start_menu_->addAction(register_username_);
  start_menu_->addAction(new_chat_);
}

} // end namespace Kaleidoscope
