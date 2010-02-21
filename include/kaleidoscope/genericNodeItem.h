#ifndef GENERICNODEITEM_H
#define GENERICNODEITEM_H

#include <QGraphicsItem>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>

namespace Kaleidoscope {

    /* Not that this item inherits from both QGraphicsItem and QObject.
       It should probably derive from a more appropriate graphics object,
       that incude QObject. */
    class GenericNodeItem : public QObject, public QGraphicsItem, public Object {
        Q_OBJECT

    public:
        GenericNodeItem(Device*, Grids::Value*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

        static GridsID requestCreate(Device*, Vec3D position);
        static void gridsCreate(Device*, Grids::Event*);

        /* Required functions from Kal::Object */
        void draw(Device*);

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        /* Not sure if these are needed.
           I think it has the width and height of the object */
        enum { BOUNDMIN = -10,
               BOUNDMAX = 23 };

        /* Overload Grids::Object function. */
        void setLocalPosition(Vec3D);
        void updateAttr(Grids::Event *);

    private:
        QPointF newPos;

        Device* d;

    };
}


#endif // GENERICNODEITEM_H
