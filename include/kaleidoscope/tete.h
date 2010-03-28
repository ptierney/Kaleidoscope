#ifndef TETE_H
#define TETE_H

#include <string>
#include <vector>

#include <grids/gObject.h>

namespace Kaleidoscope {
    class Chat;
    class TeteNode;

    class Tete : public Grids::Object {
    public:
        Tete(Device*, Grids::Value*);
        void init();

        static GridsID requestCreate(Device* dev, GridsID parent, GridsID chat, std::string text, Vec3D position);
        static void gridsCreate(Device* dev, Grids::Event* evt);

        enum { TEXT_TETE,
               TEXT_IMAGE_TETE,
               IMAGE_TETE };               

        std::string text();
        void set_text(std::string);
        std::string getTextFromAttr(Grids::Value*);
        GridsID getChatIDFromAttr(Grids::Value*);

        void set_parent(Tete*);
        void addReference(Tete*);
        void addChild(Tete*);
        Tete* parent();
        std::vector<Tete*> reference_tetes();
        std::vector<Tete*> child_tetes();
        Chat* chat();
        void set_chat(Chat*);
        GridsID chat_id();
        void set_chat_id(GridsID);
        TeteNode* tete_node();
        void set_tete_node(TeteNode*);
        std::vector<Chat*> referenced_chats();

    private:
        std::string text_;
        Device* d_;
        TeteNode* tete_node_;
        Tete* parent_;
        std::vector<Tete*> reference_tetes_;
        std::vector<Tete*> child_tetes_;
        Chat* chat_;
        GridsID chat_id_;
        // This is more for cacheing purposes
        std::vector<Chat*> referenced_chats_;

    };

}


#endif // TETE_H
