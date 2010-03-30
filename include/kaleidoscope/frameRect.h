#ifndef FRAMERECT_H
#define FRAMERECT_H


#include <QGraphicsObject>
#include <QRectF>

namespace Kaleidoscope {
  class TeteNode;
  class Device;

  class FrameRect : public QGraphicsObject {
    Q_OBJECT
  public:
    FrameRect(Device* d, QRectF rect, TeteNode*,
              QGraphicsItem* parent = 0,
              QGraphicsScene* scene = 0);

    void set_frame_rect(QRectF);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  private:
    QRectF frame_rect_;
    bool selected;
    TeteNode* tete_node_;
  };

}

#endif // FRAMERECT_H
