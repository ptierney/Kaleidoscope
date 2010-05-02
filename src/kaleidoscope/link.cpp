
#include <kaleidoscope/link.h>
#include <kaleidoscope/linkNode.h>
#include <grids/utility.h>
#include <grids/interface.h>
#include <grids/event.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  Link::Link(Device* d, Grids::Value* val) :
      Grids::Object(d, val) {
    d_ = d;
    node_1_ = NULL;
    node_2_ = NULL;
    const Grids::Value& attr = getAttrFromValue(*val);
    node_1_id_ = getNode1IDFromAttr(attr);
    node_2_id_ = getNode2IDFromAttr(attr);
    link_node_ = NULL;
  }

  void Link::init(){
    // Find nodes, and add get their pointers if they
    // exist
  }

  GridsID Link::requestCreate(Device* dev,
                              GridsID node1,
                              GridsID node2) {
    Grids::Value* create_val = new Grids::Value();
    GridsID new_id;

    (*create_val)["type"] = "Link";
    (*create_val)["node1"] = node1;
    (*create_val)["node2"] = node2;

    new_id = dev->getInterface()->requestCreateObject(create_val);
    delete create_val;
    return new_id;
  }

  void Link::gridsCreate(Device* dev, Grids::Event* evt){
    Grids::Value* val = evt->getArgsPtr();

    Link* link = new Link(dev, val);
    link->init();
    LinkNode* link_node = new LinkNode(dev);
    link_node->set_link(link);
    link->set_link_node(link_node);

    // If broken link, clean up and return
    if( dev->chat_controller()->addLink(link) == false ){
      //dev->getNoticeWindow()->write(7, "Received broken link");
      link->set_link_node(NULL);
      delete link_node;
      return;
    }

    //dev->getNoticeWindow()->write(7, "Adding link node to scene");
    dev->getScene()->addItem(link_node);
    link_node->updateBoundingRect();
    link_node->update();
  }

  GridsID Link::getNode1IDFromAttr(const Grids::Value& attr){
    return attr["node1"].asString();
  }

  GridsID Link::getNode2IDFromAttr(const Grids::Value& attr){
    return attr["node2"].asString();
  }

  GridsID Link::node_1_id(){
    return node_1_id_;
  }

  GridsID Link::node_2_id(){
    return node_2_id_;
  }

  void Link::set_node_1_id(GridsID id){
    node_1_id_ = id;
  }

  void Link::set_node_2_id(GridsID id){
    node_2_id_ = id;
  }

  bool Link::node1IsNull(){
    return node_1_ == NULL;
  }

  Tete* Link::node_1(){
      return node_1_;
  }

  Tete* Link::node_2(){
    return node_2_;
  }

  void Link::set_node_1(Tete* node){
    node_1_ = node;
  }

  void Link::set_node_2(Tete* node){
    node_2_ = node;
  }

  LinkNode* Link::link_node(){
    return link_node_;
  }

  void Link::set_link_node(LinkNode* link_node){
    link_node_ = link_node;
  }

}
