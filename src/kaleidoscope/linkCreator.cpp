
#include <iostream>

#include <QPainter>

#include <kaleidoscope/linkCreator.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  LinkCreator::LinkCreator(Device* d, QGraphicsItem* parent) :
    QGraphicsObject(parent){
    d_ = d;
    last_tete_ = "";
    last_tete_node_ = NULL;
    draw_rect_ = QRectF();
    line_weight_ = 3;
    line_color_ = QColor(255, 0, 0);
  }

  void LinkCreator::init(){

  }

  void LinkCreator::registerClick(GridsID item_id){
    //std::cerr << "Register click" << std::endl;
    if(last_tete_.empty()){
      last_tete_ = item_id;
      Tete* tete = d_->chat_controller()->getTeteFromID(item_id);
      if(tete == NULL)
        return;
      last_tete_node_ = tete->tete_node();
      updateDrawRect();
      return;
    }

    if(last_tete_ == item_id){
      last_tete_ = "";
      last_tete_node_ = NULL;
      return;
    }

    Link::requestCreate(d_, last_tete_, item_id);
    last_tete_ = "";
    last_tete_node_ = NULL;
  }

  QRectF LinkCreator::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath LinkCreator::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void LinkCreator::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {
    if(last_tete_node_ == NULL)
      return;

    updateDrawRect();
    QLineF link_line = QLineF(last_node_pos_, last_mouse_pos_);

    painter->setPen(QPen(line_color_, line_weight_, Qt::DashLine));
    painter->drawLine(link_line);
  }

  void LinkCreator::updateDrawRect(){
    // Get the center of the draw rect of node origin
    // get the cursor position
    QRectF node_rect = last_tete_node_->boundingRect();
    last_node_pos_ = node_rect.center() + last_tete_node_->pos();
    View2D* view = d_->getScene()->main_view();
    Scene2D* scene = d_->getScene();

    //last_mouse_pos_ = view->cursor().pos(); // The global mouse position
    //last_mouse_pos_ = view->mapFromGlobal(last_mouse_pos_.toPoint()); // Get the position in the window
    //last_mouse_pos_ = view->mapToScene(last_mouse_pos_.toPoint()); // Get the position in the scroll bars

    last_mouse_pos_ = view->mapToScene(view->mapFromGlobal(view->cursor().pos()));
    //std::cerr << last_mouse_pos_.x() << " : " << last_mouse_pos_.y() << std::endl;

    draw_rect_ = QRectF(last_node_pos_, last_mouse_pos_).normalized();
  }






}
