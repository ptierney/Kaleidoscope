
#include <QPainter>

#include <kaleidoscope/nameTextItem.h>

namespace Kaleidoscope {

  NameTextItem::NameTextItem(Device* d, TextNode* node,
                             QGraphicsItem* parent) :
  QGraphicsTextItem(parent) {
    d_ = d;
    node_ = node;
  }

  void NameTextItem::init(){
  }

  void NameTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    //QPainter temp_painter = QPainter(*painter);
    //painter->setPen(Qt::SolidLine);
    //painter->drawEllipse(QPoint(), 10, 10);

    QGraphicsTextItem::paint(painter, option, widget);
  }
}
