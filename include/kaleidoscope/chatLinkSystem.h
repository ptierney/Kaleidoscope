#ifndef CHATLINKSYSTEM_H
#define CHATLINKSYSTEM_H

#include <vector>

#include <QObject>
#include <QGraphicsItem>

#include <kaleidoscope/define.h>

/* The spring system that keeps some order to
   this system.
   This requires that each Tete knows both its
   parent and its references (the thats that have
   it as the parents.)
   */

namespace Kaleidoscope {
  class Device;
  class TeteNode;
  class Tete;
  class Chat;
  class Link;

  class ChatLinkSystem : public QObject {
    Q_OBJECT
  public:
    ChatLinkSystem(Device*, QObject* parent = 0);

    void update(std::vector<Chat*> chats);
    void doForces(Tete* tete, Chat* chat);
    void doChatForces(std::vector<Chat*> chats);

    bool running();
    void set_running(bool);
    void setChatRunning(GridsID chat_id);

    void setType1();
    void setType2();

  protected:
    void timerEvent(QTimerEvent*);

    Vec3D coulombRepulsion(QPointF, QPointF);
    Vec3D hookeAttraction(QPointF, QPointF,
                          float rest_distance);

  private:
    Device* d_;
    float rest_distance_;
    float dormant_rest_distance_;
    float rest_difference_;
    float chat_rest_distance_;
    float repulse_weight_;
    float attract_weight_;
    float min_velocity_;
    float max_velocity_;
    float damping_;
    float chat_damping_;
    float total_kinetic_energy_;
    std::map<GridsID, float> chat_kinetic_energy_;
    std::map<GridsID, bool> chat_running_;
    float chats_kinetic_energy_;
    bool chats_running_;
    float energy_threshold_;
    float push_dropoff_;
    bool running_;

    int force_timer_id_;
    int position_timer_id_;
  };
}

#endif // CHATLINKSYSTEM_H
