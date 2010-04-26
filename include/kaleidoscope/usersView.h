#ifndef USERSVIEW_H
#define USERSVIEW_H

#include <QGraphicsView>
#include <QFrame>

namespace Kaleidoscope {
  class UsersScene;
  class Device;

  class UsersView : public QGraphicsView {
    Q_OBJECT

  public:
    UsersView(Device* d, UsersScene* scene);

    void init();

    void drawBackground(QPainter* painter, const QRectF &rect);

    void wheelEvent(QWheelEvent*);
    void scaleView(qreal scale_factor);

  private:
    Device* d_;
  };
}



#endif // USERSVIEW_H
