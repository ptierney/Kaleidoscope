
#include <kaleidoscope/inputTete.h>
#include <grids/utility.h>
#include <grids/interface.h>
#include <grids/event.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  InputTete::InputTete(Device* d, Grids::Value* val) :
      Grids::Object(d, val) {
    d_ = d;
    chat_id_ = getChatIDFromAttr(getAttrFromValue(val));
    parent_id_ = getParentIDFromAttr(getAttrFromValue(val));
  }

  GridsID InputTete::getChatIDFromAttr(Grids::Value* attr){
    return (*attr)["chat"].asString();
  }

  GridsID InputTete::getParentIDFromAttr(Grids::Value* attr) {
    return (*attr)["parent"].asString();
  }

  GridsID InputTete::requestCreate(Device* dev,
                                   GridsID parent, GridsID chat){
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id = dev->getGridsUtility()->getNewUUID();

    (*create_val)["type"] = "InputTete";
    (*create_val)["id"] = new_id;
    (*create_val)["parent"] = parent;
    (*create_val)["chat"] = chat;

    dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  void InputTete::gridsCreate(Device* /*dev*/, Grids::Event* /*evt*/){
    /*
    Grids::Value *val = evt->getArgsPtr();

    Tete* tete = new Tete(dev, val);
    input_tete->init();
    DisplayTextNode* display_node = new DisplayTextNode(dev, tete);
    display_node->init();

    tete->set_tete_node(display_node);
    display_node->set_tete(tete);

    dev->chat_controller()->addTete(tete);
    display_node->placeNode();

    dev->getScene()->addItem(display_node);
    */

  }

  void InputTete::set_text(std::string text){
    text_ = text;
  }

  void InputTete::appendText(std::string text){
    text_ += text;
  }

}
