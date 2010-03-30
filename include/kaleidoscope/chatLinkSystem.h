#ifndef CHATLINKSYSTEM_H
#define CHATLINKSYSTEM_H

#include <vector>

#include <QObject>

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

  class ChatLinkSystem : public QObject {
    Q_OBJECT
  public:
    ChatLinkSystem(Device*, QObject* parent = 0);

    void update(const std::vector<Chat*>& chats);
    void doForces(Tete* tete, Chat* chat);
    void doPullForce(Tete* tete, Tete* other,
                     float* xvel, float* yvel);

  protected:
    void timerEvent(QTimerEvent*);

  private:
    Device* d_;
    float min_distance_;
    float pull_weight_;
    float push_weight_;
    float min_velocity_;
    float max_velocity_;
  };
}

#endif // CHATLINKSYSTEM_H
