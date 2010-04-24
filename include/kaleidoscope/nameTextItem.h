#ifndef NAMETEXTITEM_H
#define NAMETEXTITEM_H

#include <QGraphicsTextItem>
#include <QTextLayout>

// A wrapper around a QGraphicsItem that allows for display caching

namespace Kaleidoscope {
  class Device;
  class TextNode;

  class NameTextItem : public QGraphicsTextItem {
    Q_OBJECT
  public:
    NameTextItem(Device* d, TextNode* node,
                 QGraphicsItem* parent = 0);
    virtual ~NameTextItem();
    void init();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setPlainText(const QString& text);

  private:
    Device* d_;
    TextNode* node_;
    QTextLayout* layout_;

    float w_2_;
    float h_2_;
  };
}

#endif // NAMETEXTITEM_H
