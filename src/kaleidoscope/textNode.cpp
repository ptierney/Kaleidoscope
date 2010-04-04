
#include <vector>

#include <QPointF>
#include <QPainter>

#include <kaleidoscope/textNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/displayTextItem.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  TextNode::TextNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    TeteNode(d, parent, scene) {
    // Set color to black
    text_color_ = QColor(0, 0, 0);
    // If drawing a rect behind the text, set its size (in addition to the text size) here.
    rect_boarder_width_ = 0;
    rect_boarder_height_ = 0;
  }

  TextNode::~TextNode(){
    // Nothing here
  }

  void TextNode::init(){
    TeteNode::init();
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

    if(selected_){
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
      //painter->drawRect(frame_rect().translated(pos() * -1.0));
    }
  }

  /* Is there a more efficient way to do this, so that I don't have to access .x(), y(), etc.
     This function is called every loop. */
  void TextNode::updateDrawRect(){
    draw_rect_ = QRectF(text_item_->pos().x() + text_item_->boundingRect().left() - rect_boarder_width_,
                       text_item_->pos().y() + text_item_->boundingRect().top() - rect_boarder_height_,
                       text_item_->boundingRect().width() + 2*rect_boarder_width_,
                       text_item_->boundingRect().height() + 2*rect_boarder_height_);
  }

  void TextNode::centerTextItem(){
    if(text_item_ == NULL)
      return;

    text_item_->setPos(text_item_->boundingRect().width() / -2.0,
                      text_item_->boundingRect().height() / -2.0);
  }

  void TextNode::setText(std::string text){
    text_item_->setText(text);
    centerTextItem();
  }

  void TextNode::setActiveText(std::string text){
    text_item_->setActiveText(text);
  }


}
