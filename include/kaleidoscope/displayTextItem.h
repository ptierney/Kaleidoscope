#ifndef DISPLAYTEXTITEM_H
#define DISPLAYTEXTITEM_H

// A simple item to display text, though that
// can allow the user to edit the text

#include <QGraphicsTextItem>

namespace Kaleidoscope {
  class Device;
  class DisplayTextNode;

  class DisplayTextItem : public QGraphicsTextItem {
    Q_OBJECT

  public:
    DisplayTextItem(Device* d,
                    DisplayTextNode* node,
                    QGraphicsItem* parent = 0);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);

  protected:

  private:
    Device* d_;
    DisplayTextNode* node_;
  };

}

#endif // DISPLAYTEXTITEM_H
