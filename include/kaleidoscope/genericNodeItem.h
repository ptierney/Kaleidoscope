#ifndef GENERICNODEITEM_H
#define GENERICNODEITEM_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>
#include <grids/define.h>

namespace Kaleidoscope {

    /* Not that this item inherits from both QGraphicsItem and QObject.
       It should probably derive from a more appropriate graphics object,
       that incude QObject. */
    class GenericNodeItem : public QGraphicsObject, public Object {
       Q_OBJECT

    public:
        GenericNodeItem(Device*, Grids::Value*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

        static GridsID requestCreate(Device*, Vec3D position, std::string text);
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
        std::string getTextFromAttr(Grids::Value*);

        QPointF newPos;

        Device* d;

        QGraphicsTextItem *text_item;

        float rect_boarder;
        float line_thickness;
        float fill_color_r, fill_color_g, fill_color_b, fill_color_a;
        float text_r, text_g, text_b, text_a;
    };
}


#endif // GENERICNODEITEM_H
