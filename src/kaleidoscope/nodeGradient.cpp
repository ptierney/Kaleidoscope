
#include <iostream>

#include <QRadialGradient>
#include <QPainter>

#include <kaleidoscope/nodeGradient.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  NodeGradient::NodeGradient(Device* d, TeteNode* parent_node, QGraphicsItem* parent) :
      QGraphicsObject(parent) {
    d_ = d;
    parent_node_ = parent_node;
    rect_width_ = 500;
    rect_height_ = rect_width_;
    time_dropoff_ = 10 * 1000;
    alpha_refresh_ = 150;
    focus_line_weight_ = 4.0;
    update_timer_ = 0;
    updateDrawRect();
  }

  NodeGradient::~NodeGradient(){

  }

  void NodeGradient::init() {
    update_timer_ = startTimer(alpha_refresh_);
  }

  void NodeGradient::activate(){
    update_timer_ = startTimer(alpha_refresh_);
  }

  void NodeGradient::timerEvent(QTimerEvent* /*event*/) {
    update();
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

  void NodeGradient::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {
    if(update_timer_ == 0)
      activate();

    QColor grad_color = parent_node_->gradient_color();

    // TODO: Maybe there's a better way to do this?
    if(d_->chat_controller()->last_selected() &&
       d_->chat_controller()->last_selected()->tete_node() == parent_node_){
      // Check if the node is selected, if so draw an (ellipse, circle, box, dot...) to show that it is.
      // TODO: Move this somewhere more logical
      painter->setPen(QPen(grad_color, focus_line_weight_, Qt::DotLine));
      // Check out Qt::BrushStyles for more info
      //painter->setBrush(Qt::SolidPattern);
      painter->setBrush(Qt::NoBrush);
      float radius = parent_node_->boundingRect().width() > parent_node_->boundingRect().height() ? parent_node_->boundingRect().width() : parent_node_->boundingRect().height();
      painter->drawEllipse(QPoint(), (int)radius, (int)radius);

      if(parent_node_->activeElapsed() > time_dropoff_){
        killTimer(update_timer_);
        update_timer_ = 0;
        return;
      }
    }

    if(d_->chat_controller()->gradient_toggle() == false)
      return;

    float alpha_scale = (1.0 - (float)(parent_node_->activeElapsed()) / (float)time_dropoff_);
    alpha_scale = alpha_scale >= 0 ? alpha_scale : 0.0;

    grad_color.setAlpha(100.0 * alpha_scale);
    //std::cerr << alpha_scale << std::endl;
    //updateDrawRect();
    // Update colors based on activity time
    radial_gradient_ = new QRadialGradient(QPointF(), rect_width_);
    radial_gradient_->setColorAt(0, grad_color);
    radial_gradient_->setColorAt(0.5, QColor(255, 255, 255, 0));
    painter->setBrush(*radial_gradient_);
    painter->setPen(Qt::NoPen);
    painter->drawRect(draw_rect_);
    delete radial_gradient_;
    radial_gradient_ = NULL;
  }

  void NodeGradient::updateDrawRect() {
    draw_rect_ = QRectF(-rect_width_/2, -rect_height_/2, rect_width_, rect_height_);
  }

}
