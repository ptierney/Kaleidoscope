#ifndef USERSSCENE_H
#define USERSSCENE_H

#include <QGraphicsScene>

namespace Kaleidoscope {
  class UsersView;
  class Device;
  class OtherUsersNode;

  class UsersScene : public QGraphicsScene {
    Q_OBJECT

  public:
    UsersScene(Device*, QObject* parent = 0);
    virtual ~UsersScene();
    void init();

    QSize sizeHint() const;
    UsersView* users_view();
    void set_users_view(UsersView*);
    OtherUsersNode* other_users_node();

  private:
    UsersView* users_view_;
    Device* d_;
    OtherUsersNode* other_users_node_;
  };
}

#endif // USERSSCENE_H
