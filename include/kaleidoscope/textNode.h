#ifndef TEXTNODE_H
#define TEXTNODE_H

#include <QColor>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {
  class Tete;
  class DisplayTextItem;

  class TextNode : public TeteNode {
    
  public:
    TextNode(Device*, QGraphicsItem* parent = 0, QGraphicsScene* = 0);
    virtual ~TextNode();
    void init();
    
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setText(std::string);
    void setActiveText(std::string);

    DisplayTextItem* text_item();

    void centerTextItem();
  protected:
    DisplayTextItem* text_item_;
    QColor text_color_;
    QRectF draw_rect_;

    int rect_boarder_width_;
    int rect_boarder_height_;
    
    void updateDrawRect();
    bool frame_selected_;
  };
}

#endif // TEXTNODE_H
