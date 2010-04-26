
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
#include <kaleidoscope/user.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  Tete::Tete(Device* d, Grids::Value* val) :
      Grids::Object(d, val) {
    d_ = d;
    const Grids::Value& temp_attr = getAttrFromValue(*val);

    text_ = getTextFromAttr(temp_attr);
    chat_id_ = getChatIDFromAttr(temp_attr);
    chat_ = NULL;
    parent_id_ = getParentIDFromAttr(temp_attr);
    parent_ = NULL;
    owner_id_ = getOwnerIDFromAttr(temp_attr);
    user_name_ = getUserNameFromAttr(temp_attr);
    if(user_name_.empty()){
      //user_name_ = d->getUserController()->get
    } else {
      // push user name onto userControllerStack
    }
    user_color_ = getUserColorFromAttr(temp_attr);

    tete_node_ = NULL;
  }

  void Tete::init(){
    if(chat_id_.empty()){
      chat_id_ = d_->chat_controller()->default_chat_id();
    }

    if(false && owner_id_ == d_->my_id()){
      const Grids::Value& links = getLinksFromAttr(getAttr());

      GridsID other_node_id;
      for(unsigned int i = 0u; i < links.size(); ++i){
        other_node_id = links[i].asString();
        Link::requestCreate(d_,id(), other_node_id);
      }
    }
  }

  GridsID Tete::requestCreate(Device *dev,
                              GridsID parent, GridsID chat,
                              std::string text, Vec3D position){
    return Tete::requestCreate(dev, dev->user()->name(),
                               parent, chat,
                               text, position);
  }

  GridsID Tete::requestCreate(Device* dev, std::string owner_name,
                              GridsID parent, GridsID chat,
                              std::string text, Vec3D position){
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "Tete";
    (*create_val)["text"] = text;
    (*create_val)["links"][0u] = parent;
    (*create_val)["parent"] = parent;
    (*create_val)["owner"] = dev->my_id();
    (*create_val)["owner_name"] = owner_name;
    if(dev->user()->hasSetName())
      (*create_val)["user_name"] = dev->user()->name();
    (*create_val)["chat"] = chat;
    QColor user_color = dev->user()->color();
    (*create_val)["owner_color"][0u] = user_color.red();
    (*create_val)["owner_color"][1u] = user_color.green();
    (*create_val)["owner_color"][2u] = user_color.blue();
    (*create_val)["owner_color"][3u] = user_color.alpha();

    new_id = dev->getInterface()->requestCreateObject(create_val,position);
    delete create_val;
    return new_id;
  }

  void Tete::gridsCreate(Device* dev, Grids::Event* evt){
    Grids::Value* val = evt->getArgsPtr();

    Tete* tete = new Tete(dev, val);
    tete->init();
    DisplayTextNode* display_node = new DisplayTextNode(dev, tete);
    display_node->init();
    display_node->set_gradient_color(tete->user_color());

    tete->set_tete_node(display_node);
    display_node->set_tete(tete);

    dev->chat_controller()->addTete(tete);
    display_node->placeNode();
    dev->getScene()->addItem(display_node);

    // Something here is causing the items to become process-intensive
    GridsID owner_id = tete->owner_id();
    if(owner_id == dev->my_id()){
      // Request an update with any cached keys
      std::string append = dev->event_controller()->stopRecordingKeys();
      Tete::requestUpdate(dev, tete->id(), append);
      // Center item,
      display_node->beginFraming();
      // Select / focus item
      // 3 Lines remmoved for performance debugging
      display_node->text_item()->appendActiveText(append);
      display_node->text_item()->makeActive(); // <-- CAUSES NODES TO BOTTLENECK
      display_node->text_item()->textCursor().movePosition(QTextCursor::End);
      if(dev->chat_controller()->reframing())
        display_node->frameOn();
    }

    dev->users_scene()->other_users_node()->digestTete(tete);
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

  void Tete::requestUpdate(Device* dev,
                           GridsID tete_id,
                           float activate_amount){
    Grids::Value* val = new Grids::Value();

    (*val)["type"] = "Tete";
    (*val)["activate"] = activate_amount;
    (*val)["owner"] = dev->my_id();
    (*val)["id"] = tete_id;

    dev->getInterface()->requestUpdateAttr(tete_id, val);

    delete val;
  }


  void Tete::gridsUpdate(Device* dev, Grids::Event* event){
    GridsID id = event->getID();
    Tete* tete = dev->chat_controller()->getTeteFromID(id);
    // TODO: Maybe this should add a node if it doesn't exist
    if(tete == NULL || tete->tete_node() == NULL)
      return;

    Grids::Value* attr = event->getAttrPointer();
    std::string text = (*attr)["text"].asString();

    // This probably means I'm updating the text
    if((*attr)["owner"].asString() == dev->my_id() ){
      // This doesn't work...
      //tete->tete_node()->setActiveText(text);
    } else {
      if( !((*attr)["text"].empty()) )
        tete->tete_node()->setText(text);
    }

    if((*attr)["activate"].empty())
      tete->tete_node()->receiveActivate();
    else
      tete->tete_node()->receiveActivate( (*attr)["activate"].asDouble() );
  }

  std::string Tete::text(){
    return text_;
  }

  void Tete::set_text(std::string text){
    text_ = text;
  }

  std::string Tete::getTextFromAttr(const Grids::Value& attr){
    return attr["text"].asString();
  }

  GridsID Tete::getChatIDFromAttr(const Grids::Value& attr){
    return attr["chat"].asString();
  }

  GridsID Tete::getParentIDFromAttr(const Grids::Value& attr) {
    return attr["parent"].asString();
  }

  GridsID Tete::getOwnerIDFromAttr(const Grids::Value& attr) {
    return attr["owner"].asString();
  }

  const Grids::Value& Tete::getLinksFromAttr(const Grids::Value& attr) {
    return attr["links"];
  }

  std::string Tete::getUserNameFromAttr(const Grids::Value& attr) {
    return attr["user_name"].asString();
  }

  QColor Tete::getUserColorFromAttr(const Grids::Value& attr) {
    if(attr["owner_color"].empty())
      return QColor(255,255,255,0); // return transparent

    int r = attr["owner_color"][0u].asInt();
    int g = attr["owner_color"][1u].asInt();
    int b = attr["owner_color"][2u].asInt();

    return QColor(r, g, b);
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

  const std::vector<Tete*>& Tete::references(){
    return references_;
  }

  const std::vector<Tete*>& Tete::children(){
    return children_;
  }

  const std::vector<Link*>& Tete::links(){
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

  const std::vector<Chat*>& Tete::referenced_chats(){
    return referenced_chats_;
  }

  TeteNode* Tete::tete_node(){
    return tete_node_;
  }

  void Tete::set_tete_node(TeteNode* tete_node){
    tete_node_ = tete_node;
  }

  std::string Tete::user_name(){
    return user_name_;
  }

  void Tete::updateText(std::string text) {
    requestUpdate(d_, id(), text);
  }

  QColor Tete::user_color(){
    return user_color_;
  }

}
