#ifndef LINK_H
#define LINK_H

#include <grids/gObject.h>

namespace Kaleidoscope {
  class Tete;

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

    GridsID getNode1IDFromAttr(Grids::Value*);
    GridsID getNode2IDFromAttr(Grids::Value*);

  private:
    Device* d_;

    GridsID node_1_id_;
    GridsID node_2_id_;

    Tete* node_1_;
    Tete* node_2_;
  };
}

#endif // LINK_H
