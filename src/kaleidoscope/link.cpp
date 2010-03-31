
#include <kaleidoscope/link.h>


namespace Kaleidoscope {

  Link::Link(Device* d, Grids::Value* val) :
      Grids::Object(d, val) {
    d_ = d;
    node_1_ = NULL;
    node_2_ = NULL;
    node_1_id_ = getNode1IDFromAttr(getAttrFromValue(val));
    node_2_id_ = getNode2IDFromAttr(getAttrFromValue(val));
  }

  void Link::init(){
    // Find nodes, and add get their pointers if they
    // exist
  }

  GridsID Link::requestCreate(Device* dev,
                              GridsID node1,
                              GridsID node2) {
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id = dev->getGridsUtility()->getNewUUID();

    (*create_val)["type"] = "Link";
    (*create_val)["node1"] = node1;
    (*create_val)["node2"] = node2;
    (*create_val)["id"] = new_id;

    dev->getInterface()->requestCreateObject(create_val);
    detele create_val;
    return new_id;
  }

  void Link::gridsCreate(Dev* dev, Grids::Event* evt){
    Grids::Value* val = evt->getArgsPtr();

    // Find nodes
    // Create Link
    // Scene2D* scene = dev->getScene();
    // scene->addItem(display_node);

  }

  Link::getNode1IDFromAttr(Grids::Value* attr){
    return (*attr)["node1"].asString();
  }

  Link::getNode2IDFromAttr(Grids::Value* attr){
    return (*attr)["node2"].asString();
  }



}
