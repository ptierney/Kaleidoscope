#ifndef TETENODE_H
#define TETENODE_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>

namespace Kaleidoscope {
  class TeteNode : public QGraphicsObject {
    Q_OBJECT

      public:
    TeteNode(QGraphicsItem* parent = 0, QGraphicsScene* = 0);
    void init();
    

  private:
    QGraphicsTextItem* text;
  };




}

#endif // TETENODE_H
