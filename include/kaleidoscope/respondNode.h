#ifndef RESPONDNODE_H
#define RESPONDNODE_H

#include <QGraphicsObject>
#include <QGraphicsScene>

namespace Kaleidoscope {
  class Device;

  class RespondNode : public QGraphicsObject {
    Q_OBJECT
      
      public:
    RespondNode(Device*, QGraphicsItem* parent = 0, QGraphicsScene* = 0);
    virtual ~RespondNode();
    void init();

    virtual bool frameOn() = 0;

    bool selected();

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    bool selected_;
    // Used for keeping track of reframing
    long frame_counter_;
    // The space between an object and the edge of the window
    int zoom_margin_;
    float zoom_speed_;
    // This stores how often to check for reframing
    int loop_period_;
    
    Device* d_;
  };


}

#endif // RESPONDNODE_H
