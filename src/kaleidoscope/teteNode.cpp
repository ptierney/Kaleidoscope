
#include <iostream>

#include <QTimer>

#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/frameRect.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  TeteNode::TeteNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    RespondNode(d, parent, scene) {
    tete_ = NULL;
    x_vel_ = 0.0;
    y_vel_ = 0.0;
    frame_rect_ = QRectF(-1, -1, 2, 2);
    frame_rect_object_ = NULL;
    mouse_moved_ = true;
    velocity_ = Vec3D();
    dormant_ = false;
  }

  TeteNode::~TeteNode(){
    if(tete_)
      tete_->set_tete_node(NULL);
  }

  void TeteNode::init(){
    RespondNode::init();

    last_active_.start();
  }

  Tete* TeteNode::tete(){
    return tete_;
  }

  void TeteNode::set_tete(Tete* tete){
    tete_ = tete;
  }

  void TeteNode::hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/){
    //Q_UNUSED(event)
    beginFraming();
  }

  void TeteNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/){
    //Q_UNUSED(event)
    selected_ = false;
  }

  void TeteNode::beginFraming(){
    updateFrameRect();
    selected_ = true;
    frame_selected_ = true;
    mouse_moved_ = false;
    if(frame_rect_object_){
      delete frame_rect_object_;
      frame_rect_object_ = NULL;
    }
    frame_rect_object_ = new FrameRect(d_, frame_rect_, this);
    d_->getScene()->addItem(frame_rect_object_);
  }

  bool TeteNode::frameOn(){
    // Get the rects of parent, referenced, and children nodes
    updateFrameRect();
    float total_object_width = frame_rect_.width();
    float total_object_height = frame_rect_.height();
    QString string;
    string.setNum(total_object_width);
    //d_->getNoticeWindow()->write(7, tr("width = ")+string);
    string.setNum(total_object_height);
    //d_->getNoticeWindow()->write(7, tr("height = ")+string);

    float window_width = d_->main_window->centralWidget()->size().width();
    float window_height = d_->main_window->centralWidget()->size().height();

    float display_width = window_width - zoom_margin_*2.0;
    float display_height = window_height - zoom_margin_*2.0;

    float view_scale_width = display_width / total_object_width;
    float view_scale_height = display_height / total_object_height;

    float view_scale = view_scale_width <= view_scale_height ?
                       view_scale_width :
                       view_scale_height;    

    QMatrix current_matrix = d_->getScene()->main_view()->matrix();
    // Note: m11 and m22 hold the horizontal and vertical scale.
    // They should be the same.
    float current_scale = current_matrix.m11();

    float new_scale = current_scale + (view_scale - current_scale) * zoom_speed_;

    QMatrix temp_matrix;
    temp_matrix.scale(new_scale, new_scale);
    d_->getScene()->main_view()->setMatrix(temp_matrix);

    //d_->getScene()->main_view()->ensureVisible(frame_rect_, zoom_margin_, zoom_margin_);
    d_->getScene()->main_view()->centerOn(this);

    if(mouse_moved_ == false){
      mouse_moved_ = true;

      QPoint condense = d_->getScene()->main_view()->mapToGlobal(d_->getScene()->main_view()->mapFromScene(pos().toPoint()));
      d_->getScene()->main_view()->cursor().setPos(condense);
    }

    return selected_;
  }

  void TeteNode::frameLeave(FrameRect* /*frame*/){

  }

  bool TeteNode::frame_selected(){
    if(frame_rect_object_ == NULL)
      return false;

    if(frame_selected_ == false)
      return false;

    updateFrameSelected();

    return frame_selected_;
  }

  void TeteNode::updateFrameSelected(){
    frame_selected_ = frame_rect_object_->isUnderMouse();

    if(frame_selected_ == false){
      delete frame_rect_object_;
      frame_rect_object_ = NULL;
    }
  }

  void TeteNode::updatePosition(){
    setPos(x() + velocity_.X, y() + velocity_.Y);
  }

  float TeteNode::x_vel(){
    return velocity_.X;
  }

  float TeteNode::y_vel(){
    return velocity_.Y;
  }

  Vec3D TeteNode::velocity(){
    return velocity_;
  }

  void TeteNode::set_velocity(Vec3D velocity){
    velocity_ = velocity;
  }

  void TeteNode::addVelocity(Vec3D addition){
    velocity_ += addition;
  }

  QRectF TeteNode::frame_rect(){
    return frame_rect_;
  }

  void TeteNode::updateFrameRect(){
    // First set the initial min max values to this object's bounding
    // box
    QRectF local_bound = boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(pos());
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    float start_min_x, start_min_y, start_max_x, start_max_y;
    float min_x = start_min_x = bound.topLeft().x();//frame_rect_.topLeft().x();
    float min_y = start_min_y = bound.topLeft().y();//frame_rect_.topLeft().y();
    float max_x = start_max_x = bound.bottomRight().x();//frame_rect_.bottomRight().x();
    float max_y = start_max_y = bound.bottomRight().y();//frame_rect_.bottomRight().y();

    std::vector<Link*> links = tete_->links();
    Tete* other_node;
    // Should this be a const_iterator?
    for(std::vector<Link*>::const_iterator it = links.begin(); it != links.end(); ++it){
      if((*it)->node_1() == tete_)
        other_node = (*it)->node_2();
      else
        other_node = (*it)->node_1();

      addTeteToMinMax(other_node, &min_x, &min_y,
                      &max_x, &max_y);
    }

    // If nothing was added to one side, double the frame on that side,
    // so that the center node is more or less guaranteed to be centered.
    if(min_x == start_min_x)
      min_x -= (max_x - min_x);
    if(min_y == start_min_y)
      min_y -= (max_y - min_y);
    if(max_x == start_max_x)
      max_x += (max_x - min_x);
    if(max_y == start_max_y)
      max_y += (max_y - min_y);

    frame_rect_ = QRectF(QPointF(min_x, min_y),
                         QPointF(max_x, max_y)).normalized();
  }

  void TeteNode::addTeteToMinMax(Tete* tete,
                          float* min_x, float* min_y,
                          float* max_x, float* max_y) {
      //return;
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    QRectF local_bound = tete->tete_node()->boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(tete->tete_node()->pos());
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    if(bound.topLeft().x() < *min_x)
      *min_x = bound.topLeft().x();
    if(bound.topLeft().y() < *min_y)
      *min_y = bound.topLeft().y();
    if(bound.bottomRight().x() > *max_x)
      *max_x = bound.bottomRight().x();
    if(bound.bottomRight().y() > *max_y)
      *max_y = bound.bottomRight().y();
  }

  QTime* TeteNode::last_active(){
    return &last_active_;
  }

  int TeteNode::activeElapsed(){
    return last_active_.elapsed();
  }

  bool TeteNode::dormant(){
    return dormant_;
  }

  void TeteNode::activate(){
    dormant_ = false;
    last_active_.start();
  }


}
