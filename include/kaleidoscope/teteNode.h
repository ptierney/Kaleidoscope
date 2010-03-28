#ifndef TETENODE_H
#define TETENODE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include <kaleidoscope/respondNode.h>

namespace Kaleidoscope {
  class Tete;

  class TeteNode : public RespondNode {

  public:
    TeteNode(Device*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    virtual ~TeteNode();
    void init();

    Tete* tete();
    void set_tete(Tete*);

    // Required by ResponseNode
    bool frameOn();

  protected:
    Tete* tete_;
    
  private:

  };




}

#endif // TETENODE_H
