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

  spring_type_1_ = new QAction(tr("Spring type 1"), this);
  connect(spring_type_1_, SIGNAL(triggered()),
          this, SLOT(setSpringType1()));
  spring_type_2_ = new QAction(tr("Spring type 2"), this);
  connect(spring_type_2_, SIGNAL(triggered()),
          this, SLOT(setSpringType2()));
  graphics_type_1_ = new QAction(tr("Graphics type 1"), this);
  connect(graphics_type_1_, SIGNAL(triggered()),
          this, SLOT(setGraphicsType1()));
  graphics_type_2_ = new QAction(tr("Graphics type 2"), this);
  connect(graphics_type_2_, SIGNAL(triggered()),
          this, SLOT(setGraphicsType2()));
}

void Kaleidoscope::setSpringType1(){

}
void Kaleidoscope::setSpringType2(){

}
void Kaleidoscope::setGraphicsType1(){

}
void Kaleidoscope::setGraphicsType2(){

}

void Kaleidoscope::createMenus() {
  //gridsMenu = menuBar()->addMenu(tr("&Grids"));
  //gridsMenu->addAction(initAct);


  start_menu_ = menuBar()->addMenu(tr("Start here"));
  start_menu_->addAction(set_name_act_);
  start_menu_->addSeparator();
  start_menu_->addAction(register_username_);
  start_menu_->addAction(new_chat_);
  explore_menu_ = menuBar()->addMenu(tr("Explore"));
  spring_menu_ = explore_menu_->addMenu(tr("Spring Types"));
  explore_menu_->addSeparator();
  graphics_menu_ = explore_menu_->addMenu(tr("Graphics Types"));
  spring_menu_->addAction(spring_type_1_);
  spring_menu_->addAction(spring_type_2_);
  graphics_menu_->addAction(graphics_type_1_);
  graphics_menu_->addAction(graphics_type_2_);
}

} // end namespace Kaleidoscope
