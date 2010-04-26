#ifndef USERVIEW_H
#define USERVIEW_H

#include <QGraphicsView>

namespace Kaleidoscope {
  class Device;
  class Scene2D;
  class User;
  class Tete;

  class UserView : public QGraphicsView {
  public:
    UserView(Device* d, User* user, Scene2D* scene);
    virtual ~UserView();
    void init();

    void digestTete(Tete*);
    User* user();

  protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

  private:
    Device* d_;
    User* user_;
  };
}

#endif // USERVIEW_H
