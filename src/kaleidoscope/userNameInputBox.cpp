
#include <kaleidoscope/userNameInputBox.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UserNameInputBox::UserNameInputBox(Device* d, QWidget* parent)
    : QLineEdit(parent) {
    d_ = d;
    connect(this, SIGNAL( returnPressed() ),
            this, SLOT( userInput() ));
    setAlignment(Qt::AlignCenter);
    parent_ = parent;
  }

  void UserNameInputBox::userInput(){
    QString input_text = text();

    d_->user()->set_name(input_text.toStdString());
    //parent_->close();
    emit done();
  }

}
