
#include <iostream>

#include <QPainter>

#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/userIcon.h>

namespace Kaleidoscope {

  OtherUsersNode::OtherUsersNode(Device* d, QGraphicsItem* parent) :
      QGraphicsObject(parent) {
    d_ = d;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
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

  void OtherUsersNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setPen(QPen(Qt::blue, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::blue));
    painter->drawRect(draw_rect_);
    //std::cerr << "draw" << std::endl;
    //QGraphicsItem::paint(painter, option, widget);
  }

  void OtherUsersNode::updateDrawRect() {
    draw_rect_ = QRectF(-100, -100, 100, 100);
  }

  void OtherUsersNode::digestTete(Tete* tete){
    // Check to see if we know the user
    // if not, create a new UserIcon, attatch it somewhere
    //
    users_iterator_ = known_users_.find(tete->owner_id());
    if(users_iterator_ == known_users_.end()){
      // We haven't seen this user

    } else {
      // If we've seen this user, but don't know some piece of
      // information about them, such as color, etc
      // Update that information
    }
  }

}
