
#include <QtGui>
#include <QDialogButtonBox>

#include <kaleidoscope/userNameBox.h>
#include <kaleidoscope/userNameInputBox.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UserNameBox::UserNameBox(Device* d, QWidget* parent)
    : QWidget(parent) {
    d_ = d;
    input_line_ = new UserNameInputBox(d, this);
    button_box_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                       | QDialogButtonBox::Cancel);

    connect(button_box_, SIGNAL(accepted()),
            input_line_, SLOT(userInput()));
    connect(button_box_, SIGNAL(rejected()),
            parent, SLOT(close()));
    connect(input_line_, SIGNAL(done()),
            parent, SLOT(close()));
    connect(input_line_, SIGNAL(done()),
            this, SLOT(createColorBox()));

    QLabel* screen_name_label = new QLabel(this);
    screen_name_label->setText(tr("Your name:"));

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(screen_name_label);
    main_layout->addWidget(input_line_);
    main_layout->addWidget(button_box_);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(main_layout);

    input_line_->setFocus(Qt::PopupFocusReason);
  }

  void UserNameBox::createColorBox(){
    if(d_->user()->has_set_color() == false)
      d_->createColorPickWindow();
  }

}
