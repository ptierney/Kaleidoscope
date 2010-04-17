
#include <vector>
#include <iostream>

#include <QPointF>
#include <QPainter>
#include <QGraphicsObject>

#include <kaleidoscope/textNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/displayTextItem.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/nodeGradient.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  TextNode::TextNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    TeteNode(d, parent, scene) {
    // Set color to black
    text_color_ = QColor(0, 0, 0);
    name_color_ = QColor(0, 0, 0, 150);
    // If drawing a rect behind the text, set its size (in addition to the text size) here.
    rect_boarder_width_ = 0;
    rect_boarder_height_ = 0;
    name_scale_ = 0.75;
    text_active_scale_ = 2.0;
    text_dormant_scale_ = 0.75;
    text_scale_difference_ = text_active_scale_ - text_dormant_scale_;
    // This is the time from full size to smallest size
    time_cutoff_ = 10000;
    scale_velocity_ = 0.0;
    scale_damping_ = 0.01;
  }

  TextNode::~TextNode(){
    // Nothing here
  }

  void TextNode::init(){
    TeteNode::init();

    name_item_ = new QGraphicsTextItem(this);
    name_item_->setPlainText(tr(tete_->user_name().c_str()));
    name_item_->setDefaultTextColor(name_color_);
    name_item_->setScale(name_scale_);

    node_gradient_ = new NodeGradient(this, this);
  }

  DisplayTextItem* TextNode::text_item() {
    return text_item_;
  }

  QRectF TextNode::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath TextNode::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void TextNode::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){
    updateDrawRect();
    //updateTextSize();

    if(false && selected_){
      painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

      if(tete_->parent() && tete_->parent()->tete_node() ){
        QLineF parent_line = QLineF(QPointF(),
                                    tete_->parent()->tete_node()->pos() - pos() );
        painter->drawLine(parent_line);
      }

      for(unsigned int i = 0u; i < tete_->children().size(); i++ ){
        if(tete_->children()[i]->tete_node()){
          QLineF child_line = QLineF(QPointF(),
                                     tete_->children()[i]->tete_node()->pos() - pos());
          painter->drawLine(child_line);
        }
      }
      painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->drawRect(frame_rect().translated(pos() * -1.0));
      painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->drawRect(draw_rect_);
    }


    //painter->drawRect(draw_rect_);
  }

  /* Is there a more efficient way to do this, so that I don't have to access .x(), y(), etc.
     This function is called every loop. */
  void TextNode::updateDrawRect(){

    draw_rect_ = QRectF(text_item_->pos().x() + text_item_->boundingRect().left() - rect_boarder_width_ ,
                       text_item_->pos().y() + text_item_->boundingRect().top() - rect_boarder_height_ - name_item_->boundingRect().height() ,
                       text_item_->boundingRect().width() + 2*rect_boarder_width_,
                       text_item_->boundingRect().height() + name_item_->boundingRect().height() + 2*rect_boarder_height_);

    //draw_rect_ = text_item_->boundingRect().united(name_item_->boundingRect());
  }

  void TextNode::centerTextItem(){
    if(text_item_ == NULL)
      return;

    text_item_->setPos(text_item_->boundingRect().width() / -2.0,
                      text_item_->boundingRect().height() / -2.0);

    name_item_->setPos(text_item_->pos().x() +
                       text_item_->boundingRect().width() / 2.0 -
                       name_item_->boundingRect().width() / 2.0,
                       text_item_->pos().y() -
                       name_item_->boundingRect().height() );

  }

  void TextNode::updateTextSize(){
    //std::cerr << "Updating text size" << std::endl;
    float current_scale;
    int time_diff = time_cutoff_ - activeElapsed();
    if( time_diff < 0){
      current_scale = text_dormant_scale_;
    } else {
      current_scale = ((float)time_diff / (float)time_cutoff_)*text_scale_difference_ +
                      text_dormant_scale_;
    }
   // std::cerr << ((float)time_diff / (float)time_cutoff_) << std::endl;

    scale_velocity_ = current_scale - text_item_->matrix().m11();

    setScale(matrix().m11() + scale_velocity_ * scale_damping_);
    setScale(current_scale);
    update();
    //centerTextItem();
  }

  void TextNode::setText(std::string text){
    text_item_->setText(text);
    centerTextItem();
    activate();
  }

  void TextNode::setActiveText(std::string text){
    text_item_->setActiveText(text);
  }

  void TextNode::updateGeometry(){
    updateTextSize();
  }


}
