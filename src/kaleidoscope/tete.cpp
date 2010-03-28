
#include <algorithm>
#include <string>

#include <kaleidoscope/tete.h>
#include <kaleidoscope/displayTextNode.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/chatController.h>
#include <grids/utility.h>
#include <grids/gObject.h>
#include <grids/event.h>
#include <grids/interface.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

    Tete::Tete(Device* d, Grids::Value* val) :
            Grids::Object(d, val) {
        d_ = d;
        text_ = getTextFromAttr(getAttrFromValue(val));
        chat_id_ = getChatIDFromAttr(getAttrFromValue(val));
        chat_ = NULL;
    }

    void Tete::init(){
      if(chat_id_.empty()){
        chat_id_ = d_->chat_controller()->default_chat_id();
      }
    }

    GridsID Tete::requestCreate(Device *dev, GridsID parent, GridsID chat, std::string text, Vec3D position){
        Grids::Value* create_val = new Grids::Value();
        GridsID new_id = dev->getGridsUtility()->getNewUUID();

        (*create_val)["type"] = "Tete";
        (*create_val)["text"] = text;
        (*create_val)["id"] = new_id;
        (*create_val)["parent"] = parent;
        (*create_val)["chat"] = chat;

        dev->getInterface()->requestCreateObject(create_val,position);
        delete create_val;
        return new_id;
    }

    void Tete::gridsCreate(Device *dev, Grids::Event *evt){
        Grids::Value *val = evt->getArgsPtr();

        // Find chat from value
        // See if ChatNode exists. If not
        // create a new ChatNode an attatch it to the scene
        // scete->addItem, etc

        // Create a new TeteNode, add this to the node
        // add the

        Tete* tete = new Tete(dev, val);
        tete->init();

        DisplayTextNode* display_node = new DisplayTextNode(dev, tete);
        display_node->init();

        tete->set_tete_node(display_node);
        dev->chat_controller()->addTete(tete);

        Scene2D* scene = dev->getScene();        
        scene->addItem(display_node);
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

    void Tete::addReference(Tete* tete){
        // Check to make sure the pointer isn't already in the vector
        if( std::find(reference_tetes_.begin(), reference_tetes_.end(), tete) != reference_tetes_.end() )
            return;

        reference_tetes_.push_back(tete);
    }

    void Tete::addChild(Tete* tete){
        // Check to make sure the pointer isn't already in the vector
        if( std::find(child_tetes_.begin(), child_tetes_.end(), tete) != child_tetes_.end() )
            return;

        child_tetes_.push_back(tete);
    }

    Tete* Tete::parent(){
        return parent_;
    }

    std::vector<Tete*> Tete::reference_tetes(){
        return reference_tetes_;
    }

    std::vector<Tete*> Tete::child_tetes(){
        return child_tetes();
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







}
