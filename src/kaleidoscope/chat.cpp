
#include <kaleidoscope/chat.h>
#include <kaleidoscope/tete.h>
#include <grids/objectController.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/device.h>


namespace Kaleidoscope {

  Chat::Chat(Device* d, GridsID chat_id){
    d_ = d;
    chat_id_ = chat_id;
  }

  Chat::~Chat(){
    for(std::vector<Tete*>::iterator it = tetes_.begin(); it != tetes_.end(); ++it){
      delete *it;
    }
  }

  GridsID Chat::chat_id(){
    return chat_id_;
  }

  GridsID Chat::id(){
    return chat_id_;
  }

  void Chat::set_chat_id(GridsID chat_id){
    chat_id_ = chat_id;
  }

  void Chat::addTete(Tete* tete){
    tetes_.push_back(tete);
  }


  void Chat::addTeteAsTree(Tete* tete){

    Tete* parent = d_->chat_controller()->getTeteFromID(tete->parent_id());

     if(parent == NULL) {
       if(!tetes_.empty()){
        parent = tetes_.back();
      }
    }
    tete->set_parent(parent);

    if(parent){
      parent->addChild(tete);
    }

    // Have the first node of the chat drive the position
    //if(tetes_.empty() && tete->tete_node()){
    //  chat_node_->setPos(tete->tete_node()->pos());
    //}

    tetes_.push_back(tete);

    // By this time, the chat_node_ should have been initialized
    //tete->tete_node()->setParent(chat_node_);
  }

  const std::vector<Tete*>& Chat::tetes(){
    return tetes_;
  }

  void Chat::addLink(Link* link){
    links_.push_back(link);
  }

  const std::vector<Link*>& Chat::links(){
    return links_;
  }

  ChatNode* Chat::chat_node(){
    return chat_node_;
  }

  void Chat::set_chat_node(ChatNode* chat_node){
    chat_node_ = chat_node;
  }

}
