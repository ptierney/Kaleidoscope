
#include <QObject>
#include <QGraphicsTextItem>

#include <kaleidoscope/displayTextNode.h>
#include <kaleidoscope/tete.h>

namespace Kaleidoscope {

  DisplayTextNode::DisplayTextNode(Device* d, Tete* tete, QGraphicsItem* parent, QGraphicsScene* scene) :
    TextNode(d, parent, scene) {
    set_tete(tete);
    setPos( tete->getPosition().X, tete->getPosition().Y);
  }

  DisplayTextNode::~DisplayTextNode(){
    // Nothing here
  }

  void DisplayTextNode::init() {
    // Call any previous inits
    TextNode::init();

    text_item_ = new QGraphicsTextItem(this);
    text_item_->setPlainText(QObject::tr(tete_->text().c_str()));
    text_item_->setDefaultTextColor(text_color_);

    text_item_->setPos( text_item_->pos().x() - text_item_->boundingRect().width() / 2,
                       text_item_->pos().y() - text_item_->boundingRect().height() / 2 );

    updateDrawRect();
  }


}
