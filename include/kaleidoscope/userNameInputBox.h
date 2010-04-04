#ifndef USERNAMEINPUTBOX_H
#define USERNAMEINPUTBOX_H

#include <QLineEdit>
#include <QWidget>

namespace Kaleidoscope {
  class Device;

  class UserNameInputBox : public QLineEdit {
    Q_OBJECT

  public:
    UserNameInputBox(Device* d, QWidget* parent = 0);

  signals:
    void done();

  public slots:
    void userInput();

  private:
    Device* d_;
    QWidget* parent_;
  };


}


#endif // USERNAMEINPUTBOX_H
