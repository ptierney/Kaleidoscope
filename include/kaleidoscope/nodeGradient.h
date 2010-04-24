#ifndef NODEGRADIENT_H
#define NODEGRADIENT_H

#include <QGraphicsObject>
#include <QRadialGradient>
#include <QRectF>

namespace Kaleidoscope {
  class TeteNode;
  class Device;

  class NodeGradient : public QGraphicsObject {
    Q_OBJECT

  public:
    NodeGradient(Device*, TeteNode* parent_node, QGraphicsItem* parent = 0);
    virtual ~NodeGradient();

    void init();
    void updateDrawRect();
    void activate();
    bool active();
    void timerEvent(QTimerEvent *);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  private:
    Device* d_;
    TeteNode* parent_node_;
    QRadialGradient* radial_gradient_;
    QRectF draw_rect_;
    int rect_width_;
    int rect_height_;
    float time_dropoff_;
    int update_timer_;
    int alpha_refresh_;
    float focus_line_weight_;
    bool active_;

  };
}

#endif // NODEGRADIENT_H
