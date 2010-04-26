#ifndef USERICON_H
#define USERICON_H

#include <QGraphicsObject>

QT_BEGIN_NAMESPACE
class QTextLayout;
QT_END_NAMESPACE

namespace Kaleidoscope {
  class Device;
  class User;

  class UserIcon : public QGraphicsObject {
    Q_OBJECT
  public:
    UserIcon(Device* d, User* user,
             QGraphicsItem* parent = 0);
    virtual ~UserIcon();
    void init();

    void updateDrawRect();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    User* user();
    void set_user(User*);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

  private:
    Device* d_;
    User* user_;
    QRectF draw_rect_;
    QTextLayout* layout_;
    QSizeF text_size_;
  };

}

#endif // USERICON_H
