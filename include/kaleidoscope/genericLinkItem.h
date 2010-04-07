#ifndef GENERICLINKITEM_H
#define GENERICLINKITEM_H


#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsTextItem>
#include <QTime>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>
#include <grids/define.h>

/* Generic icon for a link between two nodes. */

namespace Kaleidoscope {
    class GenericNodeItem;

    /* Not that this item inherits from both QGraphicsItem and QObject.
       It should probably derive from a more appropriate graphics object,
       that incude QObject. */
    class GenericLinkItem : public QGraphicsObject, public Object {
       Q_OBJECT

    public:
        GenericLinkItem(Device*, Grids::Value*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

        enum LinkType { SOFT_LINK = 0,
                        HARD_LINK,
                        INFO_LINK,
                        BROKEN_LINK };

        static GridsID requestCreate(Device*,
                                     GridsID node1, GridsID node2, LinkType);
        static GridsID requestCreate(Device*,
                                     GridsID node1, GridsID node2, LinkType, GridsID parent);

        static void gridsCreate(Device*, Grids::Event*);

        /* Required functions from Kal::Object */
        void draw(Device *);

        QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void updateAttr(Grids::Event*);

        /* Call this function when one of the nodes has changed. */
        void nodeChanged();

        GenericNodeItem* getNode1();
        GenericNodeItem* getNode2();

        GenericLinkItem::LinkType getLinkType();

        QPointF getIntersectionPoint(GenericNodeItem*);

    protected:
        GridsID getNode1FromAttr(const Grids::Value&);
        GridsID getNode2FromAttr(const Grids::Value&);
        GenericLinkItem::LinkType getLinkTypeFromAttr(const Grids::Value&);

        virtual void loadNodeQPos();
        virtual void loadNodeRects();

    private:
        Grids::Object *node1, *node2;
        //QGraphicsObject *qnode1, *qnode2;
        GridsID node1_id, node2_id;

        Vec3D getNode1Pos();
        Vec3D getNode2Pos();
        Vec3D getNodePos(Grids::Object* node);

        Vec3D node1_pos, node2_pos;
        Vec3D node1_vel, node2_vel;
        QPointF node1_qpos, node2_qpos;
        /* What, two positions for each node? Why?
           Well, technically each node does have
           two positions: the "global" position
           paintained on the server, stored in
           databases, etc,
           and the local position, which is used
           for small-scale movements.
           */
        QRectF node1_rect, node2_rect;
        QLineF center_line;
        QLineF line;

        bool node_changed;
        QTime last_physics;

        QColor fill_color;
        float fill_color_r, fill_color_g, fill_color_b, fill_color_a;
        float line_thickness;

        //static const float SOFT_LINE = 0.6;
        //static const float HARD_LINE = 1.6;

        float rest_distance;
        //static const float SOFT_DISTANCE = 100.0;
        //static const float HARD_DISTANCE = 165.0;

        LinkType link_type;

        Device* d;
    };

}


#endif // GENERICLINKITEM_H
