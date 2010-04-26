
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/otherUsersNode.h>

namespace Kaleidoscope {

  UsersScene::UsersScene(Device* d, QObject* parent) :
      QGraphicsScene(parent) {
    d_ = d;
  }

  UsersScene::~UsersScene(){
    delete other_users_node_;
  }

  void UsersScene::init(){
    other_users_node_ = new OtherUsersNode(d_);
    other_users_node_->init();
    addItem(other_users_node_);
  }

  QSize UsersScene::sizeHint() const {
    return QSize(200, 200);
  }

  UsersView* UsersScene::users_view(){
    return users_view_;
  }

  void UsersScene::set_users_view(UsersView* view){
    users_view_ = view;
  }

  OtherUsersNode* UsersScene::other_users_node(){
    return other_users_node_;
  }

}
