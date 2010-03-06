#ifndef GENERICLINKITEM_H
#define GENERICLINKITEM_H


#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsTextItem>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>
#include <grids/define.h>

/* Generic icon for a link between two nodes. */

namespace Kaleidoscope {

    /* Not that this item inherits from both QGraphicsItem and QObject.
       It should probably derive from a more appropriate graphics object,
       that incude QObject. */
    class GenericLinkItem : public QGraphicsObject, public Object {
       Q_OBJECT

    public:
        GenericLinkItem(Device*, Grids::Value*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

        enum LinkType { SOFT_LINK = 0,
                        HARD_LINK };

        static GridsID requestCreate(Device*,
                                     GridsID node1, GridsID node2, LinkType);
        static GridsID requestCreate(Device*,
                                     GridsID node1, GridsID node2, LinkType, GridsID parent);

        static void gridsCreate(Device*, Grids::Event*);



        /* Required functions from Kal::Object */
        void draw(Device *);

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void updateAttr(Grids::Event*);

        /* Call this function when one of the nodes has changed. */
        void nodeChanged();

    protected:
        GridsID getNode1FromAttr(Grids::Value*);
        GridsID getNode2FromAttr(Grids::Value*);
        GenericLinkItem::LinkType getLinkTypeFromAttr(Grids::Value*);

    private:
        Grids::Object *node1, *node2;
        GridsID node1_id, node2_id;

        Vec3D getNode1Pos();
        Vec3D getNode2Pos();
        Vec3D getNodePos(Grids::Object* node);

        Vec3D node1_pos, node2_pos;

        QColor fill_color;
        float fill_color_r, fill_color_g, fill_color_b, fill_color_a;
        float line_thickness;

        LinkType link_type;

        Device* d;
    };

}


#endif // GENERICLINKITEM_H
