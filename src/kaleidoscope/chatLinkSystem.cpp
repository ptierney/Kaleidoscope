
#include <math.h>

#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatLinkSystem::ChatLinkSystem(Device* d, QObject* parent) :
    QObject(parent) {
    d_ = d;
    rest_distance_ = 200.0;
    // For pull, larger is slower
    attract_weight_ = 10.0;
    // For push, smaller is slower
    repulse_weight_ = 10.0;
    min_velocity_ = 0.1;
    max_velocity_ = 10.0;
    damping_ = 0.1;
    total_kinetic_energy_ = 0.0;
    energy_threshold_ = 1.0;
    running_ = false;
    startTimer(50);
  }

  bool ChatLinkSystem::running(){
    return running_;
  }

  void ChatLinkSystem::set_running(bool running){
    running_ = running;
  }

  void ChatLinkSystem::timerEvent(QTimerEvent* /*event*/){
    //d_->getNoticeWindow()->write(7, "update");
    if(running_){
      update(d_->chat_controller()->chats());
      running_ = total_kinetic_energy_ > energy_threshold_;
    }
  }

  void ChatLinkSystem::update(const std::vector<Chat *>& chats){
    total_kinetic_energy_ = 0.0;
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

    d_->getScene()->update(d_->getScene()->sceneRect());
  }

  void ChatLinkSystem::doForces(Tete* tete, Chat* chat){
    if(tete->tete_node() == NULL)
      return;

    Vec3D force = Vec3D(0.0, 0.0, 0.0);

    // Sum all the forces pushing this item away
    for(unsigned int i = 0u; i < chat->tetes().size(); i++){
      if(chat->tetes()[i] == tete)
        continue;
      if(chat->tetes()[i]->tete_node() == NULL)
        continue;

      force += coulombRepulsion(tete, chat->tetes()[i]);
    }

    std::vector<Link*> links = tete->links();
    Tete* other_node;

    for(unsigned int i = 0u; i < links.size(); i++){
      if(links[i]->node_1() == tete)
        other_node = links[i]->node_2();
      else
        other_node = links[i]->node_1();

      if(other_node == NULL)
        continue;

      force += hookeAttraction(tete, other_node, links[i]);
    }

    //tete->tete_node()->addVelocity(force*damping_);
    //if(force.getLength() > max_velocity_)
    //  force = Vec3D();

    tete->tete_node()->set_velocity(force*damping_);
    total_kinetic_energy_ += tete->tete_node()->velocity().getLengthSQ();
  }


  Vec3D ChatLinkSystem::coulombRepulsion(Tete* node_1, Tete* node_2){

    float dx = node_1->tete_node()->pos().x() -
               node_2->tete_node()->pos().x();
    float dy = node_1->tete_node()->pos().y() -
               node_2->tete_node()->pos().y();

    float r = sqrt(dx*dx + dy*dy);

    return Vec3D( (dx * repulse_weight_) / r,
                  (dy * repulse_weight_) / r,
                  0.0);
  }

  // Notice that the direction is flipped
  Vec3D ChatLinkSystem::hookeAttraction(Tete* node_1, Tete* node_2, Link* link){
    //if( link->node_1()->tete_node()->boundingRect().translated(link->node_1()->tete_node()->pos()).intersects(link->node_2()->tete_node()->boundingRect().translated(link->node_2()->tete_node()->pos()))){
   //   return Vec3D();
   // }

    //QPointF node_1_pos = link->link_node()->getNodeIntersectPosition(node_1);
    //QPointF node_2_pos = link->link_node()->getNodeIntersectPosition(node_2);
    QPointF node_1_pos = node_1->tete_node()->pos();
    QPointF node_2_pos = node_2->tete_node()->pos();

    float dx = node_2_pos.x() -
               node_1_pos.x();
    float dy = node_2_pos.y() -
               node_1_pos.y();

    float r = sqrt(dx*dx + dy*dy);
    float displacement = r - rest_distance_;

    return Vec3D(dx/r*displacement*attract_weight_,
                 dy/r*displacement*attract_weight_,
                 0.0);               
  }


}
