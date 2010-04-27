
#include <iostream>
#include <algorithm>

#include <QPainter>

#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/userView.h>
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

  void OtherUsersNode::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {

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

      std::vector<UserIcon*>::iterator it = user_order_.begin();
      user_order_.insert(it, icon);

      /*
      UserView* view = new UserView(d_, user, d_->getScene());
      view->init();
      user_views_[owner_id] = view;
      d_->createUserViewWindow(view);
      */
    } else {
      UserIcon* icon = known_users_[owner_id];
      // If we've seen this user, but don't know some piece of
      // information about them, such as color, etc
      // Update that information

      // Also, find the location in user_order_, delete it,
      // Push it to the front
      std::vector<UserIcon*>::iterator it = find(user_order_.begin(),
                                                 user_order_.end(),
                                                 icon);

      user_order_.erase(it);
      it = user_order_.begin();
      user_order_.insert(it, icon);
    }

    last_tetes_[owner_id] = tete;
    reorderList();

    //user_views_[owner_id]->digestTete(tete);
  }

  void OtherUsersNode::activateLastNode(GridsID owner_id){
    // Find the last tete / tete node we received from them
    std::map<GridsID, Tete*>::iterator it;
    it = last_tetes_.find(owner_id);
    if(it != last_tetes_.end()){
      Tete* last_tete = last_tetes_[owner_id];
      last_tete->tete_node()->activate();
    }
  }

  void OtherUsersNode::focusOnLastNode(GridsID owner_id){
    std::map<GridsID, Tete*>::iterator it;
    it = last_tetes_.find(owner_id);
    if(it != last_tetes_.end()){
      Tete* last_tete = last_tetes_[owner_id];
      last_tete->tete_node()->set_mouse_moved(false);
      last_tete->tete_node()->beginFraming();
      last_tete->tete_node()->frameOn();
      d_->getScene()->main_view()->setFocus(Qt::NoFocusReason);
    }
  }

  void OtherUsersNode::reorderList(){
    if(user_order_.empty())
      return;

    float height_module = user_order_.front()->boundingRect().height();
    float total_height = height_module
                         * 2.0
                         * user_order_.size();

    float counter = 0.0;
    for(std::vector<UserIcon*>::const_iterator it = user_order_.begin(); it != user_order_.end(); ++it){
      (*it)->setPos(0.0,
                    -total_height / 2.0 + height_module * 2 * counter);
      counter += 1.0;
    }
  }
}
