#ifndef DISPLAYTEXTNODE_H
#define DISPLAYTEXTNODE_H

#include <QGraphicsItem>
#include <QGraphicsScene>

#include <kaleidoscope/textNode.h>

namespace Kaleidoscope {
  class Device;
  class Tete;

  class DisplayTextNode : public TextNode {

  public:
    DisplayTextNode(Device*, Tete*, QGraphicsItem* parent = 0,
                    QGraphicsScene* scene= 0);
    virtual ~DisplayTextNode();
    void init();

    // Places it in the correct position according to its parent
    // Don't use this method.  Call the ChatNode to place the
    // entire tree.
    void placeNode();
    
  protected:
 
  };
}

#endif // DISPLAYTEXTNODE_H
