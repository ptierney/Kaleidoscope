
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
    rest_distance_ = 200.0;
    dormant_rest_distance_ = 200.0;
    rest_difference_ = rest_distance_ - dormant_rest_distance_;
    // For pull, larger is slower
    attract_weight_ = 5.0;
    // For push, smaller is slower
    repulse_weight_ = 50.0;
    min_velocity_ = 0.1;
    max_velocity_ = 10.0;
    damping_ = 0.02;
    total_kinetic_energy_ = 0.0;
    energy_threshold_ = 0.1;
    // After this distance away, the nodes don't push this node.
    push_dropoff_ = 800.0;
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
    QPointF point_1, point_2;
    QLineF line_between;
    float rest_distance;
    float repulse_scale = 1.0;

    // Sum all the forces pushing this item away
    std::vector<Tete*> tetes = chat->tetes();

    // Should this be a const_iterator?
    for(std::vector<Tete*>::const_iterator it = tetes.begin(); it !=  tetes.end(); ++it){
      if((*it) == tete)
        continue;
      if((*it)->tete_node() == NULL)
        continue;

      if(tete->tete_node()->dormant() ||
         (*it)->tete_node()->dormant() ){
        point_1 = tete->tete_node()->pos();
        point_2 = (*it)->tete_node()->pos();
      } else {
        line_between = LinkNode::getLineBetween(tete->tete_node(),
                                                (*it)->tete_node());
        point_1 = line_between.p1();
        point_2 = line_between.p2();
      }
      /*
      if(tete->tete_node()->boundingRect().intersects((*it)->tete_node()->boundingRect())){
        repulse_scale = 3.0;
        (*it)->tete_node()->setPos((*it)->tete_node()->pos() +
                                  QPointF(1000 - qrand() % 2000, 1000 - qrand() % 2000));
        std::cerr << "Intersect " << point_1.x() << " " << point_1.y() << std::endl
            << point_2.x() << " " << point_2.y() << std::endl;
        continue;
      }
      */

      force += coulombRepulsion(point_1,
                                point_2) * repulse_scale;
    }

    std::vector<Link*> links = tete->links();
    Tete* other_node;
    float average;

    for(std::vector<Link*>::iterator it = links.begin(); it != links.end(); ++it){

      if((*it)->node_1() == tete)
        other_node = (*it)->node_2();
      else
        other_node = (*it)->node_1();

      if(other_node == NULL)
        continue;

      if(tete->tete_node() == NULL ||
         other_node->tete_node() == NULL)
        return;

      if( tete->tete_node()->dormant() ||
          other_node->tete_node()->dormant() ){
        point_1 = tete->tete_node()->pos();
        point_2 = other_node->tete_node()->pos();
        rest_distance = rest_distance_;
      } else {
        (*it)->link_node()->updateLinkLine();
        point_1 = (*it)->link_node()->getNodeIntersectPosition(tete);
        point_2 = (*it)->link_node()->getNodeIntersectPosition(other_node);
        average = (tete->tete_node()->activeElapsed() + other_node->tete_node()->activeElapsed()) / 2.0;
        rest_distance = dormant_rest_distance_ + rest_difference_ * (1/average);
      }

      // Use points from an line intersection, to take into account the size of the
      // boxes.
      force += hookeAttraction(point_1, point_2,
                               rest_distance);
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

    if( r > push_dropoff_)
      return Vec3D();

    return Vec3D( (dx * repulse_weight_) / r,
                  (dy * repulse_weight_) / r,
                  0.0);
  }

  // Notice that the direction is flipped
  Vec3D ChatLinkSystem::hookeAttraction(QPointF node_1_pos, QPointF node_2_pos,
                                        float rest_distance){
    float dx = node_2_pos.x() -
               node_1_pos.x();
    float dy = node_2_pos.y() -
               node_1_pos.y();

    float r = sqrt(dx*dx + dy*dy);
    float displacement = r - rest_distance;

    return Vec3D(dx/r*displacement*attract_weight_,
                 dy/r*displacement*attract_weight_,
                 0.0);               
  }


}
