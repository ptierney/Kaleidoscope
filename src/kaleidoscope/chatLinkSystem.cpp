
#include <math.h>

#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatLinkSystem::ChatLinkSystem(Device* d, QObject* parent) :
    QObject(parent) {
    d_ = d;
    min_distance_ = 200.0;
    // For pull, larger is slower
    pull_weight_ = 2.0;
    // For push, smaller is slower
    push_weight_ = 0.1;
    min_velocity_ = 0.1;
    startTimer(50);
  }

  void ChatLinkSystem::timerEvent(QTimerEvent* event){
    //d_->getNoticeWindow()->write(7, "update");
    //update(d_->chat_controller()->chats());
  }

  void ChatLinkSystem::update(const std::vector<Chat *>& chats){
    // For every tete in every that, calculate forces
    for(unsigned int i = 0u; i < chats.size(); i++){
      for(unsigned int j = 0u; j < chats[i]->tetes().size(); j++){
        doForces(chats[i]->tetes()[j], chats[i]);
      }
    }

    Tete* tete;

    for(unsigned int i = 0u; i < chats.size(); i++){
      for(unsigned int j = 0u; j < chats[i]->tetes().size(); j++){
        tete = chats[i]->tetes()[j];
        if( tete->tete_node() )
          tete->tete_node()->updatePosition();
      }
    }
  }

  void ChatLinkSystem::doForces(Tete* tete, Chat* chat){
    if(tete->tete_node() == NULL)
      return;

    float xvel = 0.0;
    float yvel = 0.0;

    // Sum all the forces pushing this item away
    for(int i = 0; i < chat->tetes().size(); i++){
      if(chat->tetes()[i] == tete)
        continue;
      if(chat->tetes()[i]->tete_node() == NULL)
        continue;

      float dx = tete->tete_node()->pos().x() -
                 chat->tetes()[i]->tete_node()->pos().x();
      float dy = tete->tete_node()->pos().y() -
                 chat->tetes()[i]->tete_node()->pos().y();

      float l = sqrt(dx*dx + dy*dy);

      xvel += (dx *push_weight_) / l;
      yvel += (dy *push_weight_) / l;
    }

    // Now subtract all the forces pulling items together
    if(tete->parent())
      doPullForce(tete, tete->parent(),
                  &xvel, &yvel);
/*
    for(int i = 0; i < tete->child_tetes().size(); i++){
      doPullForce(tete, tete->child_tetes()[i],
                  &xvel, &yvel);
    }
    */

    for(int i = 0; i < tete->reference_tetes().size(); i++){
      doPullForce(tete, tete->reference_tetes()[i],
                  &xvel, &yvel);
    }

    if(qAbs(xvel) < min_velocity_)
      xvel = 0;
    if(qAbs(yvel) < min_velocity_)
      yvel = 0;

    tete->tete_node()->set_x_vel(xvel);
    tete->tete_node()->set_y_vel(yvel);
  }

  void ChatLinkSystem::doPullForce(Tete* tete, Tete* other,
                                   float* xvel, float* yvel ) {
    if(other->tete_node() == NULL)
      return;

    QPointF dir = other->tete_node()->pos() -
                  tete->tete_node()->pos();

    if(sqrt(dir.x() * dir.x() + dir.y() * dir.y()) > min_distance_ ){
      *xvel += dir.x() / pull_weight_;
      *yvel += dir.y() / pull_weight_;
    } else {
      *xvel = 0;
      *yvel = 0;
    }
  }
}
