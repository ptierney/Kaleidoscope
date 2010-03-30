#ifndef TETENODE_H
#define TETENODE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <kaleidoscope/respondNode.h>

namespace Kaleidoscope {
  class Tete;

  class TeteNode : public RespondNode {

  public:
    TeteNode(Device*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    virtual ~TeteNode();
    void init();

    Tete* tete();
    void set_tete(Tete*);
    void updatePosition();

    // Required by ResponseNode
    bool frameOn();
    float x_vel();
    void set_x_vel(float);
    float y_vel();
    void set_y_vel(float);
    QRectF frame_rect();
    void updateFrameRect();

    virtual void placeNode() = 0;

  protected:
    Tete* tete_;
    
  private:
    float x_vel_;
    float y_vel_;

    QRectF frame_rect_;

    void addTeteToMinMax(Tete*,
                            float*, float*,
                            float*, float*);

  };




}

#endif // TETENODE_H
