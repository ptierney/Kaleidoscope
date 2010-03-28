
#include <kaleidoscope/respondNode.h>

namespace Kaleidoscope {

  RespondNode::RespondNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    QGraphicsObject(parent) {
    Q_UNUSED(scene)
    d_ = d;
    zoom_margin_ = 100;
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

  /*
  void RespondNode::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event)

    if(selected_)
      frameOn();
  }
  */

  bool RespondNode::selected(){
    return selected_;
  }

  void RespondNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    selected_ = true;

    //frameOn();
  }

  void RespondNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    selected_ = false;

    //NothingHere
  }


}
