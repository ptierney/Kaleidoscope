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
    DisplayTextNode(Device*, Tete*, QGraphicsItem* parent = 0, QGraphicsScene* scene= 0);
    virtual ~DisplayTextNode();
    void init();

    void setTete(Tete* tete);
    
  protected:
 
  };
}

#endif // DISPLAYTEXTNODE_H
