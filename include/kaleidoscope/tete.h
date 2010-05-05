#ifndef TETE_H
#define TETE_H

#include <string>
#include <vector>

#include <QColor>

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

    static GridsID requestCreate(Device* dev, std::string owner_name,
                                 GridsID parent, GridsID chat,
                                 std::string text, Vec3D position);
    static GridsID requestCreate(Device* dev,
                                 GridsID parent, GridsID chat,
                                 std::string text, Vec3D position);
    static void gridsCreate(Device* dev, Grids::Event* evt);
    static void requestUpdate(Device* dev, GridsID tete_id, std::string new_text);
    static void requestUpdate(Device *dev, GridsID tete_id, float activate_amount);
    static void gridsUpdate(Device* dev, Grids::Event* evt);

    enum { TEXT_TETE,
           TEXT_IMAGE_TETE,
           IMAGE_TETE };

    std::string text();
    void set_text(std::string);
    static std::string getTextFromAttr(const Grids::Value&);
    static GridsID getChatIDFromAttr(const Grids::Value&);
    static GridsID getParentIDFromAttr(const Grids::Value&);
    static GridsID getOwnerIDFromAttr(const Grids::Value&);
    static const Grids::Value& getLinksFromAttr(const Grids::Value&);
    static std::string getUserNameFromAttr(const Grids::Value&);
    static QColor getUserColorFromAttr(const Grids::Value&);

    void set_parent(Tete*);
    void addReference(Tete*);
    void addChild(Tete*);
    void addLink(Link*);
    Tete* parent();
    GridsID parent_id();
    GridsID owner_id();
    std::string user_name();

    const std::vector<Tete*>& references();
    const std::vector<Tete*>& children();
    const std::vector<Link*>& links();

    Chat* chat();
    void set_chat(Chat*);
    GridsID chat_id();
    void set_chat_id(GridsID);
    TeteNode* tete_node();
    void set_tete_node(TeteNode*);
    const std::vector<Chat*>& referenced_chats();
    void updateText(std::string);
    QColor user_color();

    void registerFinished(std::string text);

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
    QColor user_color_;
  };

}


#endif // TETE_H
