
#include <iostream>

#include <QPainter>
#include <QLinearGradient>
#include <QTimerEvent>

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
    line_color_ = QColor(0, 0, 0, 50);
    line_weight_ = 0.5;
    link_strength_ = 1.0;
    link_speed_ = 1.0;
    attract_scale_ = 1.0;
    attract_scale_max_ = 8.0;
    // 10 seconds to total attraction
    scale_dropoff_ = 10 * 1000;
    update_timer_id_ = 0;
    update_time_ = 100;
    link_ = NULL;
    active_ = true;
  }

  void LinkNode::init(){
    activate();
    updateLinkValues();
    updateLinkLine();
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

  void LinkNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
    //return;
    updateBoundingRect();
    // This gets called in ChatLinkSystem when something changes,
    // so this probably doesn't need to be update
    // updateLinkLine();
    //painter->setRenderHint(QPainter::Antialiasing, true);
    // Any weight below 1 on OpenGL antialiasing looks bad.
    painter->setPen(QPen(line_color_, 1, Qt::SolidLine ));
    painter->drawLine(link_line_);
    /*
      Failure

    QPointF midpoint = QPointF((link_line_.p1().x() + link_line_.p2().x()) / 2.0,
                               (link_line_.p1().y() + link_line_.p2().y()) / 2.0);
    painter->setPen(QPen(line_color_, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint(midpoint);
    linear_gradient_ = new QLinearGradient();
    linear_gradient_->setStart(link_line_.p1());
    QLineF normal = link_line_.normalVector();
    QPointF normal_midpoint = QPointF((normal.p1().x() + normal.p2().x()) / 2.0,
                               (normal.p1().y() + normal.p2().y()) / 2.0);
    linear_gradient_->setFinalStop(normal_midpoint);
    linear_gradient_->setColorAt(0, QColor(0, 0, 0, 100.0 ));
    linear_gradient_->setColorAt(0.5, QColor(255, 255, 255, 0));
    painter->setBrush(*linear_gradient_);
    //painter->setPen(Qt::NoPen);

    static const QPointF points1[3] = {
         QPointF(bounding_rect_.topLeft() + midpoint),
         QPointF(bounding_rect_.topRight() + midpoint ),
         QPointF(bounding_rect_.bottomLeft() + midpoint)
     };
    static const QPointF points2[3] = {
         QPointF(bounding_rect_.bottomRight() + midpoint),
         QPointF(bounding_rect_.topRight() + midpoint),
         QPointF(bounding_rect_.bottomLeft() + midpoint)
     };


    //painter->drawRect(bounding_rect_);
    painter->drawPolygon(points2, 3);
    QPointF new_p2 = (normal.p1() + (normal.p1() - normal.p2()));
    normal.setP2(new_p2);
    normal_midpoint = QPointF((normal.p1().x() + normal.p2().x()) / 2.0,
                                   (normal.p1().y() + normal.p2().y()) / 2.0);
    linear_gradient_->setFinalStop(normal_midpoint);
    linear_gradient_->setColorAt(0, QColor(0, 0, 0, 100.0 ));
    linear_gradient_->setColorAt(0.5, QColor(255, 255, 255, 0));
    painter->setBrush(*linear_gradient_);
    //painter->setPen(Qt::NoPen);
    painter->drawPolygon(points1, 3);
    delete linear_gradient_;
    lineal_gradient_ = NULL;
    */
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

    updateLinkLine();
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
      return QPointF();
    }
  }

  void LinkNode::activate(){
    active_ = true;
  }

  void LinkNode::updateLinkValues(){
    int active_1 = link_->node_1()->tete_node()->activeElapsed();
    int active_2 = link_->node_2()->tete_node()->activeElapsed();
    int active_average = (active_1 + active_2) / 2;

    // If no active_1 + active_2 == 0, scale = 1.0
    // If active_1 + active_2 == scale_dropoff, scale = attract_scale

    if(active_average > scale_dropoff_){
      attract_scale_ = attract_scale_max_;
      return;
    }

    attract_scale_ = ((float)active_average / (float)scale_dropoff_) * (attract_scale_max_ - 1.0) + 1.0;

    if(attract_scale_ > attract_scale_max_ * 6.0 / 8.0){
      active_ = false;
    }
  }

  float LinkNode::attract_scale(){
    return attract_scale_;
  }

  bool LinkNode::active(){
    return active_;
  }

}
