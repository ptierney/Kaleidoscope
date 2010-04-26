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
  class UserView;

  class OtherUsersNode : public QGraphicsObject {
    Q_OBJECT
  public:
    OtherUsersNode(Device* d, QGraphicsItem* parent = 0);
    virtual ~OtherUsersNode();
    void init();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    void updateDrawRect();
    void digestTete(Tete*);
    void reorderList();

  public slots:
    void activateLastNode(GridsID owner_id);
    void focusOnLastNode(GridsID owner_id);

  private:
    Device* d_;
    QRectF draw_rect_;

    std::map<GridsID, UserIcon*> known_users_;
    std::map<GridsID, UserIcon*>::iterator users_iterator_;
    std::map<GridsID, Tete*> last_tetes_;
    std::vector<UserIcon*> user_order_;
    std::map<GridsID, UserView*> user_views_;
  };
}

#endif // OTHERUSERSNODE_H
