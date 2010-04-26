
#include <QPainter>

#include <kaleidoscope/frameRect.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {

  FrameRect::FrameRect(Device* d, QRectF rect, TeteNode* tete_node,
                       QGraphicsItem* parent,
                       QGraphicsScene* /*scene*/) :
  QGraphicsObject(parent) {
    d_ = d;
    //setAcceptHoverEvents(true);
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

  void FrameRect::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
  }





}
