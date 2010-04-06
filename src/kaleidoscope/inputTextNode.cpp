
#include <kaleidoscope/inputTextNode.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  InputTextNode::InputTextNode(Device* d,
                               QGraphicsItem* parent,
                               QGraphicsScene* scene) :
  TextNode(d, parent, scene) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);

    key_delay_ = 250;
    input_time_ = 2500;
  }

  void InputTextNode::init(){
    /*
    TextNode::init();

    text_item_ = new DisplayTextItem(this);
    text_item_->setDefaultTextColor(text_color_);
    text_item_->setTextInteractionFlags(Qt::TextEditorInteraction);
    //text_item_->setCursor(Qt::IBeamCursor);
    centerTextItem();
    */
  }

  void InputTextNode::keyPressEvent(QKeyEvent* /*event*/){
    //d_->getNoticeWindow()->write(7, "Key Pressed");
  }

  void InputTextNode::placeNode(){
    // Nothing here
  }




}
