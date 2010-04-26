#ifndef USERICON_H
#define USERICON_H

#include <QGraphicsObject>

QT_BEGIN_NAMESPACE
class QTextLayout;
class QAction;
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
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

  public slots:
    void createMenuCallback();
    void activateLastNode();
    void focusOnLastNode();

  private:
    Device* d_;
    User* user_;
    QRectF draw_rect_;
    QTextLayout* layout_;
    QSizeF text_size_;
    QMenu* menu_;
    QAction* show_on_map_;
    QAction* focus_on_;
    bool mouse_released_;
  };

}

#endif // USERICON_H
