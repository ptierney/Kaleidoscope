#ifndef LINK_H
#define LINK_H

// At the moment, this only supports local, non-broken
// links. Both tetes exest on this client, and
// both are in the same chat.

#include <grids/gObject.h>

namespace Kaleidoscope {
  class Tete;
  class LinkNode;

  class Link : public Grids::Object {
  public:
    Link(Device* d, Grids::Value* val);
    void init();

    static GridsID requestCreate(Device*, GridsID, GridsID);
    static void gridsCreate(Device*, Grids::Event*);

    Tete* node_1();
    Tete* node_2();
    void set_node_1(Tete*);
    void set_node_2(Tete*);

    GridsID node_1_id();
    GridsID node_2_id();
    void set_node_1_id(GridsID);
    void set_node_2_id(GridsID);

    GridsID getNode1IDFromAttr(const Grids::Value&);
    GridsID getNode2IDFromAttr(const Grids::Value&);

    LinkNode* link_node();
    void set_link_node(LinkNode*);

  private:
    Device* d_;

    GridsID node_1_id_;
    GridsID node_2_id_;

    Tete* node_1_;
    Tete* node_2_;

    LinkNode* link_node_;
  };
}

#endif // LINK_H
