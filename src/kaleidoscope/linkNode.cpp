
#include <QPainter>

#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>


namespace Kaleidoscope {

  LinkNode::LinkNode(Device* d, QGraphicsItem* parent,
                     QGraphicsScene* /*scene*/) :
  QGraphicsObject(parent) {
    d_ = d;
    line_color_ = Qt::black;
    line_weight_ = 0.15;
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

  void LinkNode::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
    updateBoundingRect();
    // This gets called in ChatLinkSystem when something changes,
    // so this probably doesn't need to be update
    //updateLinkLine();
    painter->setPen(QPen(line_color_, line_weight_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(link_line_);
  }

  void LinkNode::updateBoundingRect(){
    if(link_ == NULL ||
       link_->node_1() == NULL ||
       link_->node_2() == NULL ||
       link_->node_1()->tete_node() == NULL ||
       link_->node_2()->tete_node() == NULL){
      bounding_rect_ = QRectF();
      return;
    }

    link_line_ = QLineF(link_->node_1()->tete_node()->pos(),
                        link_->node_2()->tete_node()->pos());

    bounding_rect_ = QRectF(link_line().p1(),
                            link_line().p2()).normalized();
  }

  Link* LinkNode::link(){
    return link_;
  }

  void LinkNode::set_link(Link* link){
    link_ = link;
  }

  QLineF LinkNode::link_line(){
    return link_line_;
  }

  QLineF LinkNode::getLineBetween(TeteNode* node_1, TeteNode* node_2){
    if(node_1 == NULL || node_2 == NULL)
      return QLineF();

    QPointF node1_qpos = node_1->pos();
    QPointF node2_qpos = node_2->pos();
    QRectF node1_rect = node_1->boundingRect();
    QRectF node2_rect = node_2->boundingRect();

    if(node1_rect.intersects(node2_rect)){
      QLineF(node1_qpos, node1_qpos);
    }

    QLineF center_line = QLineF(node1_qpos,
                                 node2_qpos);

    /* Find intersection with first box. */
    QPointF p2;
    QPointF intersection_point_1, intersection_point_2;
    QLineF poly_line[4];

    poly_line[0] = QLineF( node1_rect.topLeft(), node1_rect.bottomLeft());
    poly_line[1] = QLineF( node1_rect.bottomLeft(), node1_rect.bottomRight());
    poly_line[2] = QLineF( node1_rect.bottomRight(), node1_rect.topRight());
    poly_line[3] = QLineF( node1_rect.topRight(), node1_rect.topLeft());

    for( int i = 0; i < 4; i++) {
        poly_line[i].translate(node1_qpos);
        QLineF::IntersectType intersect_type = poly_line[i].intersect(center_line, &intersection_point_1);
        if(intersect_type == QLineF::BoundedIntersection)
            break;
    }

    poly_line[0] = QLineF( node2_rect.topLeft(), node2_rect.bottomLeft());
    poly_line[1] = QLineF( node2_rect.bottomLeft(), node2_rect.bottomRight());
    poly_line[2] = QLineF( node2_rect.bottomRight(), node2_rect.topRight());
    poly_line[3] = QLineF( node2_rect.topRight(), node2_rect.topLeft());

    for( int i = 0; i < 4; i++) {
        poly_line[i].translate(node2_qpos);

        QLineF::IntersectType intersect_type = poly_line[i].intersect(center_line, &intersection_point_2);
        if(intersect_type == QLineF::BoundedIntersection)
            break;
    }

    return QLineF(intersection_point_1, intersection_point_2);
  }

  void LinkNode::updateLinkLine(){
    link_line_ = getLineBetween(link_->node_1()->tete_node(),
                                link_->node_2()->tete_node());
  }

  QPointF LinkNode::getNodeIntersectPosition(Tete* tete){    

    if(tete == link_->node_1())
      return link_line_.p1();
    else if(tete == link_->node_2())
      return link_line_.p2();
    else {
      //d_->getNoticeWindow()->write(7, "Error in LinkNode");
      return QPointF();
    }
  }

}
