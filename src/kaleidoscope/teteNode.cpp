
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
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

  void TeteNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    updateFrameRect();
    selected_ = true;
    frame_selected_ = true;
    frame_rect_object_ = new FrameRect(d_, frame_rect_, this);
    d_->getScene()->addItem(frame_rect_object_);
  }

  void TeteNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    Q_UNUSED(event)
    selected_ = false;
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
    return selected_;
  }

  void TeteNode::frameLeave(FrameRect *frame){
    frame_selected_ = false;
    delete frame;
  }

  bool TeteNode::frame_selected(){
    return frame_selected_;
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

    float min_x = bound.topLeft().x();//frame_rect_.topLeft().x();
    float min_y = bound.topLeft().y();//frame_rect_.topLeft().y();
    float max_x = bound.bottomRight().x();//frame_rect_.bottomRight().x();
    float max_y = bound.bottomRight().y();//frame_rect_.bottomRight().y();

    std::vector<Tete*> children = tete_->children();
    for(unsigned int i = 0u; i < children.size(); i++){
      addTeteToMinMax(children[i], &min_x, &min_y,
                      &max_x, &max_y);
    }

    addTeteToMinMax(tete_->parent(), &min_x, &min_y,
                    &max_x, &max_y);

    if(tete_->children().empty()){
      max_x += (max_x - min_x)*0.6;
    }

    if(tete_->parent() == NULL){
      min_x -= (max_x - min_x)*0.6;
    }

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


}
