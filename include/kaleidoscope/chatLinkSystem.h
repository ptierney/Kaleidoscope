#ifndef CHATLINKSYSTEM_H
#define CHATLINKSYSTEM_H

#include <vector>

#include <QObject>

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

    void update(const std::vector<Chat*>& chats);
    void doForces(Tete* tete, Chat* chat);

    bool running();
    void set_running(bool);

  protected:
    void timerEvent(QTimerEvent*);

    Vec3D coulombRepulsion(Tete*, Tete*);
    Vec3D hookeAttraction(Tete*, Tete*, Link*);

  private:
    Device* d_;
    float rest_distance_;
    float repulse_weight_;
    float attract_weight_;
    float min_velocity_;
    float max_velocity_;
    float damping_;
    float total_kinetic_energy_;
    float energy_threshold_;
    bool running_;
  };
}

#endif // CHATLINKSYSTEM_H
