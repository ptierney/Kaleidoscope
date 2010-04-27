#ifndef VIEW2D_H
#define VIEW2D_H

#include <QGraphicsView>

namespace Kaleidoscope {
  class Scene2D;
  class Device;
  class LinkCreator;

  class View2D : public QGraphicsView {
    Q_OBJECT

  public:
    View2D(Device* device, Scene2D* scene);

    void scaleView(qreal scaleFactor);
    void print();
    //void render(QPainter*, const QRectF&, const QRect&, Qt::AspectRatioMode);
    LinkCreator* link_creator();

  protected:
    void focusOutEvent(QFocusEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    //void drawBackground(QPainter *painter, const QRectF &rect);

    /* This is set in the constructor. */
    QRect world_size;
  private:
    Device* d_;
    LinkCreator* link_creator_;
  };
}

#endif // VIEW2D_H
