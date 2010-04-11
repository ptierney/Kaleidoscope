
#include <kaleidoscope/outsideChatController.h>
#include <grids/define.h>
#include <grids/event.h>
#include <kaleidoscope/user.h>
#include <grids/interface.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  OutsideChatController::OutsideChatController(Device* d, QObject* parent)
    : QObject(parent) {
    d_ = d;

  }

  GridsID OutsideChatController::requestAddAccount(Device* dev,
                                                   std::string protocol,
                                                   std::string screen_name,
                                                   std::string password) {
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "AddOutsideAccount";
    (*create_val)["protocol"] = protocol;
    (*create_val)["screen_name"] = screen_name;
    (*create_val)["password"] = password;
    (*create_val)["id"] = dev->my_id();
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  GridsID OutsideChatController::requestOutsideChat(Device* dev,
                                                    std::string chat,
                                                    std::string protocol,
                                                    std::string your_screen_name,
                                                    std::string other_screen_name,
                                                    std::string message) {
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "OutsideChat";
    (*create_val)["protocol"] = protocol;
    (*create_val)["send_screen_name"] = your_screen_name;
    (*create_val)["receive_screen_name"] = other_screen_name;
    (*create_val)["message"] = message;
    (*create_val)["chat"] = chat;
    (*create_val)["id"] = dev->my_id();
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;

  }

  void OutsideChatController::gridsCreateOutsideChat(Device* /*d*/,
                                                     Grids::Event* /*evt*/) {

  }

}
