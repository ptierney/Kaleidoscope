#ifndef LINKCREATOR_H
#define LINKCREATOR_H

#include <QGraphicsObject>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class TeteNode;

  class LinkCreator : public QGraphicsObject {
    Q_OBJECT
  public:
    LinkCreator(Device* d, QGraphicsItem* parent = 0);
    void init();
    void registerClick(GridsID);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateDrawRect();
    void clear();
    bool active();

  private:
    Device* d_;
    GridsID last_tete_;
    TeteNode* last_tete_node_;
    QRectF draw_rect_;
    int refresh_;
    QPointF last_mouse_pos_;
    QPointF last_node_pos_;
    float line_weight_;
    QColor line_color_;
  };

}

#endif // LINKCREATOR_H
