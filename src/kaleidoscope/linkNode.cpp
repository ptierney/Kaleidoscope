
#include <kaleidoscope/linkNode.h>

namespace Kaleidoscope {

  LinkNode::LinkNode(Device* d, QGraphicsItem* parent,
                     QGraphicsScene* scene) :
  QGraphicsObject(parent) {
    line_color_ = Qt::black;
    line_weight_ = 0.75;
    link_ = NULL;
  }

  void LinkNode::init(){
    // Nothing here
  }

  QRectF LinkNode::boundingRect() const {
    qreal adjust = 2;
    return bounding_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath LinkNode::shape() const {
    QPainterPath path;
    path.addRect(bounding_rect_);
    return path;
  }

  void LinkNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    updateBoundingRect();
    painter->setPen(QPen(line_color_, line_weight_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLink(link_line_)
  }

  void LinkNode::updateBoundingRect(){
    if(link_ == NULL ||
       link_->node_1_ == NULL ||
       link_->node_2_ == NULL ||
       link_->node_1_->tete_node() == NULL ||
       link_->node_2_->tete_node() == NULL){
      bounding_rect_ = QRectF();
      return;
    }

    link_line_ = QLineF(link_->node_1_->tete_node()->pos(),
                        link_->node_2_->tete_node()->pos());

    bounding_rect_ = QRectF(link_line_.p1(),
                            link_line_.p2()).normalized();
  }

  Link* LinkNode::link(){
    return link_;
  }

  QLineF LinkNode::link_line(){
    return link_line_;
  }

}
