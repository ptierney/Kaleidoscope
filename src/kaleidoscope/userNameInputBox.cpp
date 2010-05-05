
#include <kaleidoscope/userNameInputBox.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UserNameInputBox::UserNameInputBox(Device* d, QWidget* parent)
    : QLineEdit(parent) {
    d_ = d;
    connect(this, SIGNAL( returnPressed() ),
            this, SLOT( userInput() ));
    setAlignment(Qt::AlignLeft);
    parent_ = parent;
    //setFocus(Qt::NoFocusReason);
  }

  void UserNameInputBox::userInput(){
    QString input_text = text();

    d_->user()->set_name(input_text.toStdString());
    d_->getScene()->main_view()->setFocus(Qt::NoFocusReason);
    //parent_->close();
    emit done();
  }

}
