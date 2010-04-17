#ifndef DISPLAYTEXTNODE_H
#define DISPLAYTEXTNODE_H

#include <QGraphicsItem>
#include <QGraphicsScene>

#include <kaleidoscope/textNode.h>

namespace Kaleidoscope {
  class Device;
  class Tete;

  class DisplayTextNode : public TextNode {
    Q_OBJECT

  public:
    DisplayTextNode(Device*, Tete*, QGraphicsItem* parent = 0,
                    QGraphicsScene* scene= 0);
    virtual ~DisplayTextNode();
    void init();

    // Places it in the correct position according to its parent
    // Don't use this method.  Call the ChatNode to place the
    // entire tree.
    void placeNode();
    void makeActive();

  public slots:
    void replaceTextItem();

  protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

  };
}

#endif // DISPLAYTEXTNODE_H
