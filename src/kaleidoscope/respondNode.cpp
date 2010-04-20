
#include <QGraphicsObject>

#include <kaleidoscope/respondNode.h>

namespace Kaleidoscope {

  RespondNode::RespondNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    QGraphicsObject(parent) {
    Q_UNUSED(scene)
    d_ = d;
    zoom_margin_ = 40;
    zoom_speed_ = 0.1;
    setAcceptHoverEvents(true);
    // Set up a 100 ms loop to check
    loop_period_ = 100;
    selected_ = false;
  }

  RespondNode::~RespondNode(){
    // Nothing here
  }

  void RespondNode::init(){
    //startTimer(100);
  }

  bool RespondNode::selected(){
    return selected_;
  }

  void RespondNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    selected_ = true;

    QGraphicsObject::hoverEnterEvent(event);
    //frameOn();
  }

  void RespondNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    selected_ = false;

    QGraphicsObject::hoverLeaveEvent(event);
    //NothingHere
  }


}
