#ifndef LINKNODE_H
#define LINKNODE_H

// This object is by and large dumb,
// it is simply a visual representation of
// the link, IE a line.

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QMutex>

namespace Kaleidoscope {
  class Link;
  class Device;
  class Tete;
  class TeteNode;

  class LinkNode : public QGraphicsObject {
    Q_OBJECT
  public:
    LinkNode(Device*, QGraphicsItem* parent = 0,
             QGraphicsScene* scene = 0);
    void init();

    static QLineF getLineBetween(TeteNode*, TeteNode*);

    Link* link();
    void set_link(Link*);
    QLineF link_line();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void updateBoundingRect();
    void updateLinkLine();
    void updateLinkValues();
    void activate();
    bool active();

    QPointF getNodeIntersectPosition(Tete*);

    float attract_scale();

    QPointF pos() const;
    void setPos(const QPointF& pos);

  protected:

  private:
    Device* d_;
    Link* link_;
    QRectF bounding_rect_;
    QLineF link_line_;
    QColor line_color_;
    float line_weight_;
    float link_strength_;
    float link_speed_;
    float attract_scale_;
    float attract_scale_max_;
    int scale_dropoff_;
    QLinearGradient* linear_gradient_;
    int update_timer_id_;
    int update_time_;
    bool active_;

    QMutex* position_mutex_;
  };

}

#endif // LINKNODE_H
