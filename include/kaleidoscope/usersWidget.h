#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>

namespace Kaleidoscope {
  class Device;

  class UsersWidget : public QWidget {
  public:
    UsersWidget(Device* d, QWidget* parent = 0);
    ~UsersWidget();
    void init();

  private:
    Device* d_;

  };





}

#endif // USERSWIDGET_H
