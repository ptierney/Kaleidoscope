
#include <iostream>

#include <QPainter>

#include <kaleidoscope/nameTextItem.h>

namespace Kaleidoscope {

  NameTextItem::NameTextItem(Device* d, TextNode* node,
                             QGraphicsItem* parent) :
  QGraphicsTextItem(parent) {
    d_ = d;
    node_ = node;
    layout_ = NULL;
  }

  NameTextItem::~NameTextItem(){
    if(layout_)
      delete layout_;
  }

  void NameTextItem::init(){

  }

  //void NameTextItem::setPlainText(char*)

  void NameTextItem::setPlainText(const QString& text){
    //std::cerr << "Setting plain text" << std::endl;

    if(layout_)
      delete layout_;

    layout_ = new QTextLayout(text, font());
    QFontMetricsF fm(font());
    layout_->beginLayout();
    QTextLine line = layout_->createLine();
    //line.setNumColumns(1);
    QSizeF s = fm.boundingRect(text).size();
    // TODO: Figure out what these numbers are all about, why width/4, height/4
    line.setPosition(QPointF(s.width()/4.0, -s.height()/4.0));

    layout_->endLayout();
    layout_->setCacheEnabled(true);

    QGraphicsTextItem::setPlainText(text);
  }

  void NameTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    if(layout_)
      layout_->draw(painter, QPointF());
    else
      QGraphicsTextItem::paint(painter, option, widget);
  }
}
