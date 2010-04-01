
#include <QObject>
#include <QGraphicsTextItem>

#include <kaleidoscope/displayTextNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/displayTextItem.h>

namespace Kaleidoscope {

  DisplayTextNode::DisplayTextNode(Device* d, Tete* tete, QGraphicsItem* parent, QGraphicsScene* scene) :
      TextNode(d, parent, scene) {
    set_tete(tete);
    setPos( tete->getPosition().X, tete->getPosition().Y);
    //setFlag(QGraphicsItem::ItemIsFocusable);
  }

  DisplayTextNode::~DisplayTextNode(){
    // Nothing here
  }

  void DisplayTextNode::init() {
    // Call any previous inits
    TextNode::init();

    text_item_ = new DisplayTextItem(d_, this, this);
    text_item_->setPlainText(QObject::tr(tete_->text().c_str()));
    text_item_->setDefaultTextColor(text_color_);

    //text_item_->setPos( text_item_->pos().x() - text_item_->boundingRect().width() / 2,
    //                   text_item_->pos().y() - text_item_->boundingRect().height() / 2 );

    centerTextItem();
    updateDrawRect();
  }

  // Don't use this!
  // Call the ChatNode to place the entire tree
  void DisplayTextNode::placeNode(){
    if(tete_->parent() && tete_->parent()->tete_node() ){
      setPos(tete_->parent()->tete_node()->pos().x() +
             text_item_->boundingRect().width() +
             tete_->parent()->tete_node()->boundingRect().width(),
             tete_->parent()->tete_node()->pos().y() +
             -4.0 * (text_item_->boundingRect().height()) );
    }
  }

  void DisplayTextNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    if (text_item_->textInteractionFlags() == Qt::NoTextInteraction){
      text_item_->setFlag(QGraphicsItem::ItemIsFocusable);
      text_item_->setFlag(QGraphicsItem::ItemIsSelectable);
      text_item_->setTextInteractionFlags(Qt::TextEditorInteraction);
    }

    //text_item_->mouseDoubleClickEvent(event);
    QGraphicsObject::mouseDoubleClickEvent(event);
  }

  void DisplayTextNode::focusInEvent(QFocusEvent* /*event*/) {

  }

  void DisplayTextNode::focusOutEvent(QFocusEvent *event) {
    centerTextItem();
    text_item_->setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsObject::focusOutEvent(event);
  }

}
