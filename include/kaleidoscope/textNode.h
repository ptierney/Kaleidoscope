#ifndef TEXTNODE_H
#define TEXTNODE_H

#include <QColor>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {
  class Tete;

  class TextNode : public TeteNode {
    
  public:
    TextNode(Device*, QGraphicsItem* parent = 0, QGraphicsScene* = 0);
    virtual ~TextNode();
    void init();
    
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QGraphicsTextItem* text_item();
  protected:
    QGraphicsTextItem* text_item_;
    QColor text_color_;
    QRectF draw_rect_;

    int rect_boarder_width_;
    int rect_boarder_height_;
    
    void updateDrawRect();
  };
}

#endif // TEXTNODE_H