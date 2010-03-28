
#include <QPointF>

#include <kaleidoscope/textNode.h>
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

  QGraphicsTextItem* TextNode::text_item() {
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

  void TextNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    updateDrawRect();
  }

  /* Is there a more efficient way to do this, so that I don't have to access .x(), y(), etc.
     This function is called every loop. */
  void TextNode::updateDrawRect(){
    draw_rect_ = QRectF(text_item_->pos().x() + text_item_->boundingRect().left() - rect_boarder_width_,
                       text_item_->pos().y() + text_item_->boundingRect().top() - rect_boarder_height_,
                       text_item_->boundingRect().width() + 2*rect_boarder_width_,
                       text_item_->boundingRect().height() + 2*rect_boarder_height_);
  }


}
