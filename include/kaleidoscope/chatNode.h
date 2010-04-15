#ifndef CHATNODE_H
#define CHATNODE_H

#include <kaleidoscope/define.h>
#include <kaleidoscope/respondNode.h>

namespace Kaleidoscope {
  class Chat;
  class ChatLinkSystem;
  class Tete;

  class ChatNode : public RespondNode {
  public:
    ChatNode(Device*, Chat* chat,
             QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    void init();

    // Adds a tete
    void appendTete();
    Tete* getAppendNode();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateDrawRect();
    void updateAllTetesRect();
    bool frameOn();

    Chat* chat();
    ChatLinkSystem* link_system();

    void buildNodeTree();
    void buildNodeTree(Tete*);
    void playbackTeteList(Tete*);
    void placeNodes();
    void placeNodes(Tete* tete);

    Vec3D& velocity();
    void set_velocity(Vec3D);
    void addVelocity(Vec3D);
    void updatePosition();

  private:
    Chat* chat_;
    ChatLinkSystem* link_system_;
    QRectF draw_rect_;
    QRectF all_tetes_rect_;

    float node_y_space_;

    void addTeteToMinMax(Tete*,
                         float*, float*, float*, float*);

    Vec3D velocity_;

  };
}

#endif // CHATNODE_H
