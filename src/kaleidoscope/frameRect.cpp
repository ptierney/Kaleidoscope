
#include <QPainter>

#include <kaleidoscope/frameRect.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {

  FrameRect::FrameRect(Device* d, QRectF rect, TeteNode* tete_node,
                       QGraphicsItem* parent,
                       QGraphicsScene* scene) :
  QGraphicsObject(parent) {
    setAcceptHoverEvents(true);
    frame_rect_ = rect;
    selected = true;
    tete_node_ = tete_node;
  }

  void FrameRect::set_frame_rect(QRectF rect){
    frame_rect_ = rect;
  }

  QRectF FrameRect::boundingRect() const {
    qreal adjust = 2;
    return frame_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath FrameRect::shape() const {
    QPainterPath path;
    path.addRect(frame_rect_);
    return path;
  }

  void FrameRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawRect(frame_rect_);
  }

  // This shouldn't be called
  void FrameRect::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
  }

  void FrameRect::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    tete_node_->frameLeave(this);
  }



}
