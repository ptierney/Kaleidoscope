#ifndef USERNAMEBOX_H
#define USERNAMEBOX_H

#include <QtGui>
#include <QDialogButtonBox>

namespace Kaleidoscope {
  class Device;
  class UserNameInputBox;

  class UserNameBox : public QWidget {
    Q_OBJECT
  public:
    UserNameBox(Device* d, QWidget* parent = 0);

  private:
    Device* d_;
    UserNameInputBox* input_line_;
    QDialogButtonBox* button_box_;

  };


}


#endif // USERNAMEBOX_H
