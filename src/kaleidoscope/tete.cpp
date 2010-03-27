
#include <algorithm>

#include <kaleidoscope/tete.h>
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
        delete tete;
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

    Chat* Tete::parent_chat(){
        return parent_chat_;
    }

    std::vector<Chat*> Tete::referenced_chats(){
        return referenced_chats_;
    }







}
