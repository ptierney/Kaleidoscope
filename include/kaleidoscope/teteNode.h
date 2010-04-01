#ifndef TETENODE_H
#define TETENODE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <kaleidoscope/respondNode.h>
#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Tete;
  class FrameRect;

  class TeteNode : public RespondNode {

  public:
    TeteNode(Device*, QGraphicsItem* parent = 0,
             QGraphicsScene* scene = 0);
    virtual ~TeteNode();
    void init();

    Tete* tete();
    void set_tete(Tete*);
    void updatePosition();

    // Required by ResponseNode
    bool frameOn();
    float x_vel();
    float y_vel();
    Vec3D velocity();
    void set_velocity(Vec3D);
    void addVelocity(Vec3D);
    QRectF frame_rect();
    void updateFrameRect();

    virtual void placeNode() = 0;
    virtual void frameLeave(FrameRect*);
    bool frame_selected();

  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    Tete* tete_;
    QRectF frame_rect_;

    void updateFrameSelected();
    
  private:
    float x_vel_;
    float y_vel_;
    Vec3D velocity_;

    void addTeteToMinMax(Tete*,
                            float*, float*,
                            float*, float*);
    bool frame_selected_;
    FrameRect* frame_rect_object_;
    bool mouse_moved_;

  };




}

#endif // TETENODE_H
