
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
    std::map<std::string, std::string>& screen_names = dev->outside_chat_controller()->screen_names();
    std::map<std::string, GridsID>& chat_ids = dev->outside_chat_controller()->chat_ids();

    screen_names[protocol] = screen_name;
    chat_ids[protocol] = dev->getGridsUtility()->getNewUUID();

    Chat* chat = new Chat(dev, chat_ids[protocol]);
    dev->chat_controller()->addChat(chat);

    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

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
    std::map<std::string, GridsID>& chat_ids = dev->outside_chat_controller()->chat_ids();

    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "OutsideChat";
    (*create_val)["protocol"] = protocol;
    (*create_val)["send_screen_name"] = your_screen_name;
    (*create_val)["receive_screen_name"] = other_screen_name;
    (*create_val)["message"] = message;
    (*create_val)["chat"] = chat_ids[protocol];
    (*create_val)["id"] = dev->my_id();
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  void OutsideChatController::gridsCreateOutsideChat(Device* dev,
                                                     Grids::Event* evt) {
    std::cerr << "Creating outside chat" << std::endl;

    std::map<std::string, std::string>& screen_names = dev->outside_chat_controller()->screen_names();
    std::map<std::string, GridsID>& chat_ids = dev->outside_chat_controller()->chat_ids();

    const Grids::Value& attr = evt->getAttr();
    std::string protocol = attr["protocol"].asString();
    std::string receiver = attr["receiver_screen_name"].asString();
    std::string parent_id = "";
    Tete* parent_tete = NULL;
    std::map<std::string, std::string>::iterator proto_iter;

    proto_iter = screen_names.find(protocol);
    // If this message isn't for us, return.
    if(proto_iter == screen_names.end()){
      std::cerr << "Not for us 1" << std::endl;
      return;
    }
    else if(screen_names[protocol] != receiver){
      std::cerr << "Not for us 2: " << screen_names[protocol] << " -- " << receiver << std::endl;
      return;
    }

    std::string sender = attr["send_screen_name"].asString();
    GridsID chat_id = chat_ids[protocol];
    std::cerr << "Chat id = " << chat_id << std::endl;
    Chat* chat = dev->chat_controller()->getChatFromID(chat_id);
    if(chat){
      if(!chat->tetes().empty()){
        parent_tete = chat->tetes().back();
        if(parent_tete)
          parent_id = parent_tete->id();
      }
    } else { // This is probably a new conversation, create a new chat for it


    }

    std::string message = attr["message"].asString();

    GridsID new_node_id = Tete::requestCreate(dev,
                                              parent_id, chat_id,
                                              message, Vec3D() );

    if(parent_tete)
      Link::requestCreate(dev, new_node_id, parent_id);
  }

  std::map<std::string, std::string>& OutsideChatController::screen_names(){
    return screen_names_;
  }

  std::map<std::string, GridsID>& OutsideChatController::chat_ids(){
    return chat_ids_;
  }



}
