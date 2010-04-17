
#include <QRadialGradient>
#include <QPainter>

#include <kaleidoscope/nodeGradient.h>

namespace Kaleidoscope {

  NodeGradient::NodeGradient(TeteNode* parent_node, QGraphicsItem* parent) :
      QGraphicsItem(parent) {
    parent_node_ = parent_node;
    rect_width_ = 300;
    rect_height_ = rect_width_;
    updateDrawRect();
  }

  QRectF NodeGradient::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath NodeGradient::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void NodeGradient::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
    return;
    //updateDrawRect();
    // Update colors based on activity time
    radial_gradient_ = new QRadialGradient(QPointF(), rect_width_);
    radial_gradient_->setColorAt(0, QColor(0, 0, 0, 100));
    radial_gradient_->setColorAt(0.4, QColor(255, 255, 255, 0));
    painter->setBrush(*radial_gradient_);
    painter->setPen(Qt::NoPen);
    painter->drawRect(draw_rect_);
  }

  void NodeGradient::updateDrawRect() {
    draw_rect_ = QRectF(-rect_width_/2, -rect_height_/2, rect_width_, rect_height_);
  }

}
