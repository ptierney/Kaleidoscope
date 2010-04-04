
#include <algorithm>
#include <string>

#include <QTextCursor>

#include <kaleidoscope/tete.h>
#include <kaleidoscope/displayTextNode.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/chatController.h>
#include <grids/utility.h>
#include <grids/gObject.h>
#include <grids/event.h>
#include <grids/interface.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/displayTextItem.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  Tete::Tete(Device* d, Grids::Value* val) :
      Grids::Object(d, val) {
    d_ = d;
    text_ = getTextFromAttr(getAttrFromValue(val));
    chat_id_ = getChatIDFromAttr(getAttrFromValue(val));
    chat_ = NULL;
    parent_id_ = getParentIDFromAttr(getAttrFromValue(val));
    parent_ = NULL;
    owner_id_ = getOwnerIDFromAttr(getAttrFromValue(val));
    tete_node_ = NULL;
  }

  void Tete::init(){
    if(chat_id_.empty()){
      chat_id_ = d_->chat_controller()->default_chat_id();
    }

    if(false && owner_id_ == d_->my_id()){
      Grids::Value* links = getLinksFromAttr(getAttr());

      GridsID other_node_id;
      for(unsigned int i = 0u; i < links->size(); ++i){
        other_node_id = (*links)[i].asString();
        Link::requestCreate(d_,id(), other_node_id);
      }
    }
  }

  GridsID Tete::requestCreate(Device *dev,
                              GridsID parent, GridsID chat,
                              std::string text, Vec3D position){
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "Tete";
    (*create_val)["text"] = text;
    (*create_val)["links"][0u] = parent;
    (*create_val)["parent"] = parent;
    (*create_val)["owner"] = dev->my_id();
    (*create_val)["chat"] = chat;


    new_id = dev->getInterface()->requestCreateObject(create_val,position);
    delete create_val;
    return new_id;
  }




  void Tete::gridsCreate(Device* dev, Grids::Event* evt){
    Grids::Value *val = evt->getArgsPtr();

    Tete* tete = new Tete(dev, val);
    tete->init();
    DisplayTextNode* display_node = new DisplayTextNode(dev, tete);
    display_node->init();

    tete->set_tete_node(display_node);
    display_node->set_tete(tete);

    dev->chat_controller()->addTete(tete);
    display_node->placeNode();

    dev->getScene()->addItem(display_node);

    GridsID owner_id = tete->owner_id();
    if(owner_id == dev->my_id()){
      // Request an update with any cached keys
      std::string append = dev->event_controller()->stopRecordingKeys();
      Tete::requestUpdate(dev, tete->id(), append);
      // Center item,
      display_node->beginFraming();
      // Select / focus item
      //display_node->makeActive();
      display_node->text_item()->appendActiveText(append);
      display_node->text_item()->makeActive();
      display_node->text_item()->textCursor().movePosition(QTextCursor::End);
    }
  }

  void Tete::requestUpdate(Device* dev,
                           GridsID tete_id,
                           std::string new_text){
    Grids::Value* val = new Grids::Value();

    (*val)["type"] = "Tete";
    (*val)["text"] = new_text;
    (*val)["owner"] = dev->my_id();
    (*val)["id"] = tete_id;

    dev->getInterface()->requestUpdateAttr(tete_id, val);

    delete val;
  }

  void Tete::gridsUpdate(Device* dev, Grids::Event* event){
    GridsID id = event->getID();
    Tete* tete = dev->chat_controller()->getTeteFromID(id);

    Grids::Value* attr = event->getAttrPointer();
    std::string text = (*attr)["text"].asString();

    // This probably means I'm updating the text
    if((*attr)["owner"].asString() == dev->my_id() ){
      // This doesn't work...
      //tete->tete_node()->setActiveText(text);
    } else {
      tete->tete_node()->setText(text);
    }

  }

  std::string Tete::text(){
    return text_;
  }

  void Tete::set_text(std::string text){
    text_ = text;
  }

  std::string Tete::getTextFromAttr(Grids::Value* attr){
    return (*attr)["text"].asString();
  }

  GridsID Tete::getChatIDFromAttr(Grids::Value* attr){
    return (*attr)["chat"].asString();
  }

  GridsID Tete::getParentIDFromAttr(Grids::Value* attr) {
    return (*attr)["parent"].asString();
  }

  GridsID Tete::getOwnerIDFromAttr(Grids::Value* attr) {
    return (*attr)["owner"].asString();
  }

  Grids::Value* Tete::getLinksFromAttr(Grids::Value* attr) {
    return &((*attr)["links"]);
  }

  void Tete::addReference(Tete* tete){
    // Check to make sure the pointer isn't already in the vector
    if( std::find(references_.begin(), references_.end(), tete) != references_.end() )
      return;

    references_.push_back(tete);
  }

  void Tete::addChild(Tete* tete){
    // Check to make sure the pointer isn't already in the vector
    if( std::find(children_.begin(), children_.end(), tete) != children_.end() )
      return;

    children_.push_back(tete);
  }

  void Tete::addLink(Link *link){
    links_.push_back(link);
  }

  Tete* Tete::parent(){
    return parent_;
  }

  void Tete::set_parent(Tete *parent){
    parent_ = parent;
  }

  GridsID Tete::parent_id(){
    return parent_id_;
  }

  GridsID Tete::owner_id(){
    return owner_id_;
  }

  std::vector<Tete*> Tete::references(){
    return references_;
  }

  std::vector<Tete*> Tete::children(){
    return children_;
  }

  std::vector<Link*> Tete::links(){
    return links_;
  }

  Chat* Tete::chat(){
    return chat_;
  }

  void Tete::set_chat(Chat *chat){
    chat_ = chat;
  }

  GridsID Tete::chat_id(){
    return chat_id_;
  }

  void Tete::set_chat_id(GridsID chat_id){
    chat_id_ = chat_id;
  }

  std::vector<Chat*> Tete::referenced_chats(){
    return referenced_chats_;
  }

  TeteNode* Tete::tete_node(){
    return tete_node_;
  }

  void Tete::set_tete_node(TeteNode* tete_node){
    tete_node_ = tete_node;
  }

  void Tete::updateText(std::string text) {
    requestUpdate(d_, id(), text);
  }

}
