#ifndef LINKNODE_H
#define LINKNODE_H

// This object is by and large dumb,
// it is simply a visual representation of
// the link, IE a line.

#include <QGraphicsObject>

namespace Kaleidoscope {

  class LinkNode : public QGraphicsObject {
  public:
    LinkNode(Device*, QGraphicsItem* parent = 0,
             QGraphicsScene* scene = 0);

    void init();
    Link* link();
    void set_link(Link*);
    QLineF link_line();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateBoundingRect();

  private:
    Link* link_;
    QRectF bounding_rect_;
    QLineF link_line_;
    QColor line_color_;
    float line_weight_;
  };

}

#endif // LINKNODE_H
