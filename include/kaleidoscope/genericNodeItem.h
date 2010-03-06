#ifndef GENERICNODEITEM_H
#define GENERICNODEITEM_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>
#include <grids/define.h>

/* Generic node representing text data. */

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


        /* stores if the link is selected. This may not be needed. */


        /* Overload Grids::Object function. */
        void setLocalPosition(Vec3D);
        void updateAttr(Grids::Event *);

        void addLink(GridsID);
        void addLinkPointer(Grids::Object*);
        void addLinkID(GridsID);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

        QVariant itemChange(GraphicsItemChange change, const QVariant &value);


    private:
        std::string getTextFromAttr(Grids::Value*);

        QRectF draw_rect;
        void updateDrawRect();

        QPointF newPos;
        QPointF position_change;

        Device* d;

        QGraphicsTextItem *text_item;

        float rect_boarder_width;
        float rect_boarder_height;
        float rect_radius_width;
        float rect_radius_height;

        float line_thickness;

        float fill_color_r, fill_color_g, fill_color_b, fill_color_a;

        int non_active_a;
        int non_active_r;
        int non_active_g;
        int non_active_b;

        float text_r, text_g, text_b, text_a;

        bool selected;
        bool node_1_describe, node_2_describe;

        int current_r, current_g, current_b, current_a;

        std::vector<Grids::Object*> link_pointers;
        std::vector<GridsID> link_ids;
    };
}


#endif // GENERICNODEITEM_H
