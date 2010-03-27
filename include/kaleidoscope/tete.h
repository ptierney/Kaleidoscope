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

        static GridsID requestCreate(Device* dev, GridsID parent, GridsID chat, std::string text, Vec3D position);
        static void gridsCreate(Device* dev, Grids::Event* evt);

        std::string text();
        void set_text(std::string);
        std::string getTextFromAttr(Grids::Value*);

        void set_parent(Tete*);
        void addReference(Tete*);
        void addChild(Tete*);
        Tete* parent();
        std::vector<Tete*> reference_tetes();
        std::vector<Tete*> child_tetes();
        Chat* parent_chat();
        TeteNode* tete_node();
        std::vector<Chat*> referenced_chats();

    private:
        std::string text_;
        Device* d_;
        TeteNode* tete_node_;
        Tete* parent_;
        std::vector<Tete*> reference_tetes_;
        std::vector<Tete*> child_tetes_;
        Chat* parent_chat_;
        // This is more for cacheing purposes
        std::vector<Chat*> referenced_chats_;

    };

}


#endif // TETE_H
