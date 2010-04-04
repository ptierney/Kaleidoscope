
#include <math.h>

#include <iostream>

#include <QLineF>

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
    rest_distance_ = 75.0;
    // For pull, larger is slower
    attract_weight_ = 10.0;
    // For push, smaller is slower
    repulse_weight_ = 50.0;
    min_velocity_ = 0.1;
    max_velocity_ = 10.0;
    damping_ = 0.02;
    total_kinetic_energy_ = 0.0;
    energy_threshold_ = 0.01;
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
    if( running_){
      update(d_->chat_controller()->chats());
      running_ = total_kinetic_energy_ > energy_threshold_;
    }
  }

  void ChatLinkSystem::update(std::vector<Chat*> chats){
    total_kinetic_energy_ = 0.0;

    /* Node: the variable tetes is REQUIRED. Qt segfaults otherwise. I do not know the reason,
       though it has something to do with "temporaries". */
    std::vector<Tete*> tetes;

    // For every tete in every that, calculate forces
    for(std::vector<Chat*>::iterator chat_it = chats.begin(); chat_it != chats.end(); ++chat_it){
      tetes = (*chat_it)->tetes();
      for(std::vector<Tete*>::iterator tete_it = tetes.begin(); tete_it != tetes.end(); ++tete_it){
        doForces((*tete_it), (*chat_it));
      }
    }

    for(std::vector<Chat*>::iterator chat_it = chats.begin(); chat_it != chats.end(); ++chat_it){
      tetes = (*chat_it)->tetes();
      for(std::vector<Tete*>::iterator tete_it = tetes.begin(); tete_it != tetes.end(); ++tete_it){
        if( (*tete_it)->tete_node() != NULL ){
          (*tete_it)->tete_node()->updatePosition();
        }
      }
    }

    d_->getScene()->update(d_->getScene()->sceneRect());
  }

  void ChatLinkSystem::doForces(Tete* tete, Chat* chat){
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    Vec3D force = Vec3D();

    // Sum all the forces pushing this item away
    std::vector<Tete*> tetes = chat->tetes();

    // Should this be a const_iterator?
    for(std::vector<Tete*>::const_iterator it = tetes.begin(); it !=  tetes.end(); ++it){
      if((*it) == tete)
        continue;
      if((*it)->tete_node() == NULL)
        continue;

      QLineF line_between = LinkNode::getLineBetween(tete->tete_node(),
                                                     (*it)->tete_node());

      force += coulombRepulsion(line_between.p1(),
                                line_between.p2());
    }

    std::vector<Link*> links = tete->links();
    Tete* other_node;

    for(std::vector<Link*>::iterator it = links.begin(); it != links.end(); ++it){

      if((*it)->node_1() == tete)
        other_node = (*it)->node_2();
      else
        other_node = (*it)->node_1();

      if(other_node == NULL)
        continue;

      (*it)->link_node()->updateLinkLine();

      // Use points from an line intersection, to take into account the size of the
      // boxes.
      force += hookeAttraction((*it)->link_node()->getNodeIntersectPosition(tete),
                               (*it)->link_node()->getNodeIntersectPosition(other_node));
    }

    //tete->tete_node()->addVelocity(force*damping_);
    //if(force.getLength() > max_velocity_)
    //  force = Vec3D();

    // Check for floating point NaNs
    if( force != force ){
      force = Vec3D();
    }

    tete->tete_node()->set_velocity(force*damping_);
    total_kinetic_energy_ += tete->tete_node()->velocity().getLengthSQ();
  }


  Vec3D ChatLinkSystem::coulombRepulsion(QPointF node_1_pos,
                                         QPointF node_2_pos){

    float dx = node_1_pos.x() -
               node_2_pos.x();
    float dy = node_1_pos.y() -
               node_2_pos.y();

    float r = sqrt(dx*dx + dy*dy);

    return Vec3D( (dx * repulse_weight_) / r,
                  (dy * repulse_weight_) / r,
                  0.0);
  }

  // Notice that the direction is flipped
  Vec3D ChatLinkSystem::hookeAttraction(QPointF node_1_pos, QPointF node_2_pos){
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
