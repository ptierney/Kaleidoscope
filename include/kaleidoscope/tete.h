#ifndef TETE_H
#define TETE_H

#include <string>
#include <vector>

#include <grids/gObject.h>
#include <kaleidoscope/teteInterface.h>

namespace Kaleidoscope {
  class Chat;
  class TeteNode;
  class Link;

  class Tete : public Grids::Object {
  public:
    Tete(Device*, Grids::Value*);
    void init();

    static GridsID requestCreate(Device* dev, GridsID parent, GridsID chat,
                                 std::string text, Vec3D position);
    static void gridsCreate(Device* dev, Grids::Event* evt);
    static void requestUpdate(Device* dev, GridsID tete_id, std::string new_text);
    static void gridsUpdate(Device* dev, Grids::Event* evt);

    enum { TEXT_TETE,
           TEXT_IMAGE_TETE,
           IMAGE_TETE };

    std::string text();
    void set_text(std::string);
    static std::string getTextFromAttr(Grids::Value*);
    static GridsID getChatIDFromAttr(Grids::Value*);
    static GridsID getParentIDFromAttr(Grids::Value*);
    static GridsID getOwnerIDFromAttr(Grids::Value*);
    static Grids::Value* getLinksFromAttr(Grids::Value*);
    static std::string getUserNameFromAttr(Grids::Value*);

    void set_parent(Tete*);
    void addReference(Tete*);
    void addChild(Tete*);
    void addLink(Link*);
    Tete* parent();
    GridsID parent_id();
    GridsID owner_id();
    std::string user_name();
    std::vector<Tete*> references();
    std::vector<Tete*> children();
    std::vector<Link*> links();
    Chat* chat();
    void set_chat(Chat*);
    GridsID chat_id();
    void set_chat_id(GridsID);
    TeteNode* tete_node();
    void set_tete_node(TeteNode*);
    std::vector<Chat*> referenced_chats();
    void updateText(std::string);

  private:
    std::string text_;
    std::string user_name_;
    Device* d_;
    TeteNode* tete_node_;
    Tete* parent_;
    GridsID parent_id_;
    GridsID owner_id_;
    std::vector<Tete*> references_;
    std::vector<Tete*> children_;
    std::vector<Link*> links_;
    Chat* chat_;
    GridsID chat_id_;
    // This is more for cacheing purposes
    std::vector<Chat*> referenced_chats_;

  };

}


#endif // TETE_H
