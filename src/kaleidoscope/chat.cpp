
#include <kaleidoscope/chat.h>
#include <kaleidoscope/tete.h>
#include <grids/objectController.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  Chat::Chat(Device* d, GridsID chat_id){
    d_ = d;
    chat_id_ = chat_id;
  }

  GridsID Chat::chat_id(){
    return chat_id_;
  }

  void Chat::set_chat_id(GridsID chat_id){
    chat_id_ = chat_id;
  }

  void Chat::addTete(Tete* tete){
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

    tetes_.push_back(tete);
  }

  std::vector<Tete*> Chat::tetes(){
    return tetes_;
  }

}
