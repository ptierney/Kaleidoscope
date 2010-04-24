#ifndef NAMETEXTITEM_H
#define NAMETEXTITEM_H

#include <QGraphicsTextItem>

// A wrapper around a QGraphicsItem that allows for display caching

namespace Kaleidoscope {
  class Device;
  class TextNode;

  class NameTextItem : public QGraphicsTextItem {
    Q_OBJECT
  public:
    NameTextItem(Device* d, TextNode* node,
                 QGraphicsItem* parent = 0);
    void init();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  private:
    Device* d_;
    TextNode* node_;
  };
}

#endif // NAMETEXTITEM_H
