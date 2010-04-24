#ifndef TEXTNODE_H
#define TEXTNODE_H

#include <QColor>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {
  class Tete;
  class DisplayTextItem;
  class NodeGradient;
  class NameTextItem;

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
    void updateGeometry();
    void updateTextSize();

    void hoverEnter();
    void hoverLeave();

    DisplayTextItem* text_item();

    void centerTextItem();
  protected:
    DisplayTextItem* text_item_;
    NameTextItem* name_item_;
    NodeGradient* node_gradient_;
    QColor text_color_;
    QColor name_color_;
    float name_scale_;
    float text_active_scale_;
    float text_dormant_scale_;
    float text_scale_difference_;
    float scale_velocity_;
    float scale_damping_;
    int time_cutoff_;
    QRectF draw_rect_;

    int rect_boarder_width_;
    int rect_boarder_height_;
    float text_width_;
    
    void updateDrawRect();
    bool frame_selected_;
  };
}

#endif // TEXTNODE_H
