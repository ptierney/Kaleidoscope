
#include <kaleidoscope/displayTextItem.h>
#include <kaleidoscope/displayTextNode.h>

namespace Kaleidoscope {

  DisplayTextItem::DisplayTextItem(Device* d,
                                   DisplayTextNode* node,
                                   QGraphicsItem* parent) :
  QGraphicsTextItem(parent) {
    d_ = d;
    node_ = node;

  }

  void DisplayTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    if (textInteractionFlags() == Qt::NoTextInteraction){
      setFlag(QGraphicsItem::ItemIsFocusable);
      setFlag(QGraphicsItem::ItemIsSelectable);
      setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    //text_item_->mouseDoubleClickEvent(event);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
  }

  void DisplayTextItem::focusInEvent(QFocusEvent* /*event*/) {

  }

  void DisplayTextItem::focusOutEvent(QFocusEvent *event) {
    node_->centerTextItem();
    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
  }


}
