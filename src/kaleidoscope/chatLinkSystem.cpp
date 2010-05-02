
#include <math.h>

#include <iostream>

#include <QLineF>

#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatLinkSystem::ChatLinkSystem(Device* d, QObject* parent) :
    QThread(parent) {
    d_ = d;
    setType1();
    running_ = false;
    position_timer_id_ = startTimer(50);
  }

  ChatLinkSystem::~ChatLinkSystem(){

  }

  void ChatLinkSystem::setType1(){
    rest_distance_ = 150.0;
    dormant_rest_distance_ = 100.0;
    rest_difference_ = rest_distance_ - dormant_rest_distance_;
    chat_rest_distance_ = 1000.0;

    // For attract_weight_, larger is larger, faster, stronger
    attract_weight_ = 10.0;
    // Scale for the all chats system
    all_chats_attract_scale_ = 1.0;
    // For repulse_weight_, larger is larger, faster, stronger
    repulse_weight_ = 55.0;
    all_chats_repulse_scale_ = 1.0;
    // I'm not using min/max at the moment.
    min_velocity_ = 0.1;
    max_velocity_ = 1000.0;
    damping_ = 0.02;
    chat_damping_ = 0.008;
    total_kinetic_energy_ = 0.0;
    energy_threshold_ = 0.6;
    all_chats_energy_threshold_ = 0.1;
    // After this distance away, the nodes don't push this node.
    push_dropoff_ = 800.0;
  }

  void ChatLinkSystem::setType2(){

  }

  bool ChatLinkSystem::running(){
    return running_;
  }

  void ChatLinkSystem::set_running(bool running){
    running_ = running;
  }

  void ChatLinkSystem::setChatRunning(GridsID chat_id){
    running_ = true;
    chat_running_[chat_id] = true;
    chats_running_ = true;
  }

  // Todo: this needs to be looked at, the true || should be replaced
  void ChatLinkSystem::run(){
    //if(running_ && d_->chat_controller()->spring_toggle()){
    if(running_ && spring_toggle_cache_) {
      //update(d_->chat_controller()->chats());
      update(chats_cache_);

      running_ = false;
      for(std::map<GridsID, float>::const_iterator it = chat_kinetic_energy_.begin(); it != chat_kinetic_energy_.end(); ++it){
        if(it->second > energy_threshold_){
          chat_running_[it->first] = true;
          running_ = true;
        }
        if(chats_kinetic_energy_ > all_chats_energy_threshold_){
          chats_running_ = true;
          running_ = true;
        }
      }
    } else {
      //std::cerr << "Stop" << std::endl;
    }
  }

  void ChatLinkSystem::update(const std::vector<Chat*>& chats){
    chats_kinetic_energy_ = 0.0;
    for(std::map<GridsID, float>::iterator it = chat_kinetic_energy_.begin(); it != chat_kinetic_energy_.end(); ++it){
      it->second = 0.0;
    }

    for(std::vector<Chat*>::const_iterator chat_it = chats.begin(); chat_it != chats.end(); ++chat_it){
      if(chat_running_[(*chat_it)->chat_id()] == false)
        continue;

      const std::vector<Tete*>& tetes = (*chat_it)->tetes();
      for(std::vector<Tete*>::const_iterator tete_it = tetes.begin(); tete_it != tetes.end(); ++tete_it){
        doForces((*tete_it), (*chat_it));
      }

      for(std::vector<Tete*>::const_iterator tete_it = tetes.begin(); tete_it != tetes.end(); ++tete_it){
        if( (*tete_it)->tete_node() != NULL ){
          (*tete_it)->tete_node()->updatePosition();
          //(*tete_it)->tete_node()->update();
        }
      }

      const std::vector<Link*>& links = (*chat_it)->links();
      for(std::vector<Link*>::const_iterator link_it = links.begin(); link_it != links.end(); ++link_it){
        if((*link_it)->link_node() != NULL && (*link_it)->link_node()->active()){
          (*link_it)->link_node()->updateLinkValues();
          //(*link_it)->link_node()->update();
        }
      }

      //(*chat_it)->chat_node()->updatePosition();
      // Todo change this to tete_node()->update( tete_node()->boundingRect()
      //d_->getScene()->update(d_->getScene()->sceneRect());
    }

    if(chats_running_)
      doChatForces(chats);
  }

  void ChatLinkSystem::doForces(Tete* tete, Chat* chat){
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    Vec3D force = Vec3D();
    QPointF point_1, point_2;
    QLineF line_between;

    // Sum all the forces pushing this item away
    std::vector<Tete*> tetes = chat->tetes();

    for(std::vector<Tete*>::const_iterator it = tetes.begin(); it !=  tetes.end(); ++it){
      if((*it) == tete)
        continue;
      if((*it)->tete_node() == NULL)
        continue;

      line_between = LinkNode::getLineBetween(tete->tete_node(),
                                              (*it)->tete_node());
      point_1 = line_between.p1();
      point_2 = line_between.p2();

      force += coulombRepulsion(point_1,
                                point_2);
    }

    std::vector<Link*> links = tete->links();
    Tete* other_node;

    for(std::vector<Link*>::const_iterator it = links.begin(); it != links.end(); ++it){

      if((*it)->node_1() == tete)
        other_node = (*it)->node_2();
      else
        other_node = (*it)->node_1();

      if(other_node == NULL)
        continue;

      if(tete->tete_node() == NULL ||
         other_node->tete_node() == NULL)
        continue;

      if(tete->chat() != other_node->chat())
        continue;


      (*it)->link_node()->updateLinkValues();

      float attract_scale = 1.0;
      if((*it)->link_node()){
        attract_scale = (*it)->link_node()->attract_scale();
      }

      // Use points from a line intersection, to take into account the size of the text boxes.
      (*it)->link_node()->updateLinkLine();
      point_1 = (*it)->link_node()->getNodeIntersectPosition(tete);
      point_2 = (*it)->link_node()->getNodeIntersectPosition(other_node);

      force += hookeAttraction(point_1, point_2,
                               rest_distance_ / attract_scale);
    }

    //tete->tete_node()->addVelocity(force*damping_);
    //if(force.getLength() > max_velocity_)
    //   force = Vec3D();

    // Check for floating point NaNs
    if( force != force ){
      force = Vec3D();
    }

    tete->tete_node()->set_velocity(force*damping_);
    chat_kinetic_energy_[chat->chat_id()] += tete->tete_node()->velocity().getLength();
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

    return Vec3D((dx/r)*displacement*attract_weight_,
                 (dy/r)*displacement*attract_weight_,
                 0.0);               
  }

  void ChatLinkSystem::doChatForces(const std::vector<Chat*>& chats){
    Vec3D force = Vec3D();
    ChatNode* chat_node;
    QPointF point_1, point_2;
    QLineF between_line;
    QRectF bound_1, bound_2;

    for(std::vector<Chat*>::const_iterator it = chats.begin(); it != chats.end(); ++it){
      force = Vec3D();
      chat_node = (*it)->chat_node();
      for(std::vector<Chat*>::const_iterator inner = chats.begin(); inner != chats.end(); ++inner){
        if(chat_node == (*inner)->chat_node())
          continue;

        bound_1 = chat_node->boundingRect();
        bound_2 = (*inner)->chat_node()->boundingRect();

        point_1 = bound_1.center();
        point_2 = bound_2.center();

        force += coulombRepulsion(point_1,
                                  point_2) * all_chats_repulse_scale_;

        //point_2 = QPointF();
        force += hookeAttraction(point_1, point_2,
                                 chat_rest_distance_) * all_chats_attract_scale_;

        // Check for floating point NaNs
        if( force != force ){
          force = Vec3D();
        }

        chat_node->set_velocity(force*chat_damping_);
        chats_kinetic_energy_ += chat_node->velocity().getLength();
      }

      //force += coulombRepulsion(point_1,
      //                          QPointF()) * all_chats_repulse_scale_;
    }
  }

  // New chats may be added while this is running, therefore we should only act on
  // a copy of the current chats.
  void ChatLinkSystem::set_chats_cache(const std::vector<Chat*>& chats){
    chats_cache_ = chats;
  }

  void ChatLinkSystem::set_spring_toggle_cache_(bool spring){
    spring_toggle_cache_ = spring;
  }
}
