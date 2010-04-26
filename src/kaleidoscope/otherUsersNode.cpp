
#include <iostream>

#include <QPainter>

#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  OtherUsersNode::OtherUsersNode(Device* d, QGraphicsItem* parent) :
      QGraphicsObject(parent) {
    d_ = d;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
  }

  OtherUsersNode::~OtherUsersNode(){
    for(users_iterator_ = known_users_.begin(); users_iterator_ != known_users_.end(); ++users_iterator_){
      delete users_iterator_->second;
    }
  }

  void OtherUsersNode::init(){
    updateDrawRect();
  }

  QRectF OtherUsersNode::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust);
  }

  QPainterPath OtherUsersNode::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void OtherUsersNode::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* option, QWidget* widget) {

  }

  void OtherUsersNode::updateDrawRect() {
    draw_rect_ = QRectF(-100, -100, 100, 100);
  }

  void OtherUsersNode::digestTete(Tete* tete){
    // Check to see if we know the user
    // if not, create a new UserIcon, attatch it somewhere
    //
    GridsID owner_id = tete->owner_id();
    users_iterator_ = known_users_.find(owner_id);
    if(users_iterator_ == known_users_.end()){
      // We haven't seen this user
      User* user = new User(d_, owner_id);
      user->set_color(tete->user_color());
      user->set_name(tete->user_name());
      UserIcon* icon = new UserIcon(d_, user);
      icon->init();
      d_->users_scene()->addItem(icon);
      icon->setPos(0.0,
                   icon->boundingRect().height() * known_users_.size() * 2);
      known_users_[owner_id] = icon;
    } else {
      // If we've seen this user, but don't know some piece of
      // information about them, such as color, etc
      // Update that information
    }
  }

}
