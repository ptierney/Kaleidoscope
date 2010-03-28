
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/teteNode.h>
#include <grids/objectController.h>
#include <grids/utility.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatController::ChatController(Device* d){
    d_ = d;
    chat_refresh_ = 100;
  }

  ChatController::~ChatController(){
    Chat* temp_chat;
    for(int i = 0; i < chats_.size(); i++){
      temp_chat = chats_[i];
      delete temp_chat;
    }
  }

  void ChatController::init(){
    default_chat_id_ = d_->getGridsUtility()->getNewUUID();
  }

  void ChatController::addChat(Chat* chat){
  }

  void ChatController::addTete(Tete* tete){
    // Extract Chat
    Chat* chat = tete->chat();

    // Check if Chat is known
    // If not, create a new chat
    if(chat == NULL){
      // This should be the only place I make a new Chat
      GridsID chat_id = tete->chat_id();

      for(int i = 0; i < chats_.size(); i++){
        if(chats_[i]->chat_id() == chat_id)
          chat = chats_[i];
      }

      if(chat == NULL){
        chat = new Chat(d_, chat_id);
        chats_.push_back(chat);
      }

      tete->set_chat(chat);
    }

    chat->addTete(tete);
    tetes_.push_back(tete);
  }

  void ChatController::checkReframe(){
    bool reframing = false;

    for(int i = 0; i < tetes_.size(); i++){
      if( tetes_[i] && tetes_[i]->tete_node() ){
        if( tetes_[i]->tete_node()->frameOn() ){
          reframing = true;
        }
      }
    }

    if(reframing)
      return;

    // Zoom out
  }

  GridsID ChatController::default_chat_id(){
    return default_chat_id_;
  }

  void ChatController::set_default_chat_id(GridsID default_chat_id){
    default_chat_id_ = default_chat_id;
  }

} // end namespace
