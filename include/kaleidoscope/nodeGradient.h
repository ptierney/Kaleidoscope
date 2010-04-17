#ifndef NODEGRADIENT_H
#define NODEGRADIENT_H

#include <QGraphicsItem>
#include <QRectF>

namespace Kaleidoscope {
  class TeteNode;

  class NodeGradient : public QGraphicsItem {

  public:
    NodeGradient(TeteNode* parent_node, QGraphicsItem* parent = 0);

    void updateDrawRect();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  private:
    TeteNode* parent_node_;
    QRadialGradient* radial_gradient_;
    QRectF draw_rect_;
    int rect_width_;
    int rect_height_;

  };
}

#endif // NODEGRADIENT_H
