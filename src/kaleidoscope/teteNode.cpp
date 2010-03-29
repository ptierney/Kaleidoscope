
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  TeteNode::TeteNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    RespondNode(d, parent, scene) {
    tete_ = NULL;
    x_vel_ = 0.0;
    y_vel_ = 0.0;
  }

  TeteNode::~TeteNode(){
    if(tete_)
      tete_->set_tete_node(NULL);
  }

  void TeteNode::init(){
    RespondNode::init();
  }

  Tete* TeteNode::tete(){
    return tete_;
  }

  void TeteNode::set_tete(Tete* tete){
    tete_ = tete;
  }

  bool TeteNode::frameOn(){
     // boundingRect defined in TextNode, ImageNode, etc
    QRectF my_rect = boundingRect();
    //d_->getNoticeWindow()->write(7, QObject::tr("FrameOn"));
    // Get the rects of parent, referenced, and children nodes
    int total_object_width = boundingRect().width();
    int total_object_height = boundingRect().height();

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

    d_->getScene()->main_view()->ensureVisible(this, zoom_margin_, zoom_margin_);

    return selected_;
  }

  void TeteNode::updatePosition(){
    setPos(x() + x_vel_, y() + y_vel_);
  }

  float TeteNode::x_vel(){
    return x_vel_;
  }

  void TeteNode::set_x_vel(float x_vel){
    x_vel_ = x_vel;
  }

  float TeteNode::y_vel(){
    return y_vel_;
  }

  void TeteNode::set_y_vel(float y_vel){
    y_vel_ = y_vel;
  }

}
