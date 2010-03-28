
#include <kaleidoscope/chat.h>

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
    tetes_.push_back(tete);
  }

  std::vector<Tete*> Chat::tetes(){
    return tetes_;
  }

}
