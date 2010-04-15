
#include <iostream>

#include <kaleidoscope/outsideChatController.h>
#include <grids/define.h>
#include <grids/event.h>
#include <kaleidoscope/user.h>
#include <grids/interface.h>
#include <kaleidoscope/tete.h>
#include <grids/utility.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/device.h>

/* For each screen name you register, it creates a new chat, chat ID, chat node.
   Each protocol / screen name Alice@AIM gets its own chat id.
   In the future, every time it receives a message from a person, it
   goes into that chat.
 */

namespace Kaleidoscope {

  OutsideChatController::OutsideChatController(Device* d, QObject* parent)
    : QObject(parent) {
    d_ = d;

  }

  void OutsideChatController::init(){

  }

  GridsID OutsideChatController::requestAddAccount(Device* dev,
                                                   std::string protocol,
                                                   std::string screen_name,
                                                   std::string password) {
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id; // The ID of the request object

    (*create_val)["type"] = "AddOutsideAccount";
    (*create_val)["protocol"] = protocol;
    (*create_val)["screen_name"] = screen_name;
    (*create_val)["password"] = password;
    (*create_val)["owner"] = dev->my_id();
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  GridsID OutsideChatController::requestOutsideChat(Device* dev,
                                                    std::string protocol,
                                                    std::string your_screen_name,
                                                    std::string other_screen_name,
                                                    std::string message) {
    GridsID chat_id; // The ID of the newly created chat
    GridsID new_id; // The ID of the request object (yes, requests are objects)
    Grids::Value* create_val = new Grids::Value();

    if( dev->outside_chat_controller()->chat_ids()[protocol][other_screen_name].empty() ){
      chat_id = dev->getGridsUtility()->getNewUUID();
    } else {
      chat_id = dev->outside_chat_controller()->chat_ids()[protocol][other_screen_name];
    }

    (*create_val)["type"] = "OutsideChat";
    (*create_val)["protocol"] = protocol;
    (*create_val)["send_screen_name"] = your_screen_name;
    (*create_val)["receive_screen_name"] = other_screen_name;
    (*create_val)["message"] = message;
    (*create_val)["chat"] = chat_id;
    (*create_val)["id"] = dev->my_id();
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  void OutsideChatController::gridsCreateOutsideChat(Device* dev,
                                                     Grids::Event* evt) {

    const Grids::Value& attr = evt->getAttr();
    std::string protocol = attr["protocol"].asString();
    std::string receiver = attr["receiver_screen_name"].asString();
    std::string sender_name = attr["sender_screen_name"].asString();
    std::string parent_id = "";
    Tete* parent_tete = NULL;
    Chat* chat = NULL;
    GridsID chat_id;

    if( dev->outside_chat_controller()->chat_ids()[protocol][sender_name].empty()){
      chat_id = dev->getGridsUtility()->getNewUUID();
      dev->outside_chat_controller()->chat_ids()[protocol][sender_name] = chat_id;
    } else {
      chat_id = dev->outside_chat_controller()->chat_ids()[protocol][sender_name];
      chat = dev->chat_controller()->getChatFromID(chat_id);
    }

    if(chat && !chat->tetes().empty()){
      parent_tete = chat->tetes().back();
      if(parent_tete)
        parent_id = parent_tete->id();
    }

    std::string message = attr["message"].asString();

    GridsID new_node_id = Tete::requestCreate(dev,
                                              parent_id, chat_id,
                                              message, Vec3D() );

    if(parent_tete)
      Link::requestCreate(dev, new_node_id, parent_id);
  }

  /*
  std::map<std::string, std::string>& OutsideChatController::screen_names(){
    return screen_names_;
  }

  std::map<std::string, GridsID>& OutsideChatController::chat_ids(){
    return chat_ids_;
  }
  */
  /*
  ProtocolChatPointerMap& OutsideChatController::chat_pointers(){
    return chat_pointers_;
  }
  */

  ProtocolChatIDMap& OutsideChatController::chat_ids(){
    return chat_ids_;
  }


}
