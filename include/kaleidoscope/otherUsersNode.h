#ifndef OTHERUSERSNODE_H
#define OTHERUSERSNODE_H

// This node, which is usually anchored to the main
// window, displays all the other known users, and their colors.

// When a message from a new user is received, it creates a new user icon,
// and manages stuff acociated with it.

#include <map>

#include <QGraphicsObject>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Tete;
  class UserIcon;

  class OtherUsersNode : public QGraphicsObject {
  public:
    OtherUsersNode(Device* d, QGraphicsItem* parent = 0);
    virtual ~OtherUsersNode();
    void init();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    void updateDrawRect();
    void digestTete(Tete*);

  private:
    Device* d_;
    QRectF draw_rect_;

    std::map<GridsID, UserIcon*> known_users_;
    std::map<GridsID, UserIcon*>::iterator users_iterator_;
  };
}

#endif // OTHERUSERSNODE_H
