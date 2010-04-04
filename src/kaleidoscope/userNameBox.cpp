
#include <QtGui>
#include <QDialogButtonBox>

#include <kaleidoscope/userNameBox.h>
#include <kaleidoscope/userNameInputBox.h>

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

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(input_line_);
    main_layout->addWidget(button_box_);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(main_layout);

    input_line_->setFocus(Qt::PopupFocusReason);
  }

}
