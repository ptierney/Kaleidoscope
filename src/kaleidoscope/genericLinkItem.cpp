
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QRect>
#include <QGraphicsObject>
#include <QGraphicsItem>

#include <kaleidoscope/genericLinkItem.h>
#include <kaleidoscope/genericNodeItem.h>
#include <grids/utility.h>
#include <grids/event.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <grids/interface.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

    GenericLinkItem::GenericLinkItem(Device* d, Grids::Value* val,
                                     QGraphicsItem *parent, QGraphicsScene *scene)
        :  QGraphicsObject(parent), Object(d, val) {
        this->d = d;

        d->getNoticeWindow()->write(tr("Creating generic link item"));

        Grids::Value* attr = getAttrFromValue(val);
        node1_id = getNode1FromAttr(attr);
        node2_id = getNode2FromAttr(attr);       

        node1 = d->getObjectController()->getPointerFromID(node1_id);
        node2 = d->getObjectController()->getPointerFromID(node2_id);

        if(node1 == 0 || node2 == 0){
            d->getErrorWindow()->write(7, tr("Error getting node pointers for link: ") + tr(getID().c_str()));
        }

        link_type = getLinkTypeFromAttr(attr);

        /* Check link type, set parameters. */
        fill_color = QColor(0, 0, 0);

        if( link_type == SOFT_LINK){
            line_thickness = 0.6;
            rest_distance = 100.0;
        } else if( link_type == HARD_LINK ) {
            line_thickness = 1.6;
            rest_distance = 165.0;
        }

        node1_pos = getNode1Pos();
        node2_pos = getNode2Pos();

        ((GenericNodeItem*)node1)->addLinkPointer(this);
        ((GenericNodeItem*)node2)->addLinkPointer(this);

        last_physics.start();
    }

    /* Links node1 (parent) to node2 (child) */
    GridsID GenericLinkItem::requestCreate(Device *dev, GridsID node1, GridsID node2, LinkType type){
        return GenericLinkItem::requestCreate(dev, node1, node2, type, node1);
    }

    GridsID GenericLinkItem::requestCreate(Device *dev, GridsID req_node1, GridsID req_node2, LinkType type, GridsID parent) {
        Grids::Value* create_val = new Grids::Value();
        GridsID new_id = dev->getGridsUtility()->getNewUUID();

        (*create_val)["type"] = "GenericLink";
        (*create_val)["parent"] = parent;
        (*create_val)["id"] = new_id;
        (*create_val)["node1"] = req_node1;
        (*create_val)["node2"] = req_node2;
        (*create_val)["link_type"] = type;

        return dev->getInterface()->requestCreateObject(create_val);

        delete create_val;

        return new_id;
    }

    void GenericLinkItem::gridsCreate(Device* dev, Grids::Event* evt) {
        Scene2D* scene = dev->getScene();
        Grids::Value *val = evt->getArgsPtr();

        GenericLinkItem *generic_link = new GenericLinkItem(dev, val);

        scene->addItem(generic_link);
        scene->addLinkItem(generic_link);
    }

    GridsID GenericLinkItem::getNode1FromAttr(Grids::Value* attr){
        return (*attr)["node1"].asString();
    }

    GridsID GenericLinkItem::getNode2FromAttr(Grids::Value* attr){
        return (*attr)["node2"].asString();
    }

    GenericLinkItem::LinkType GenericLinkItem::getLinkTypeFromAttr(Grids::Value *attr) {
        if( (*attr)["link_type"].asInt() == SOFT_LINK )
            return SOFT_LINK;
        else if( (*attr)["link_type"].asInt() == HARD_LINK )
            return HARD_LINK;
        else {
            d->getNoticeWindow()->write(5, tr("Error, couldn't find link type"));
            return HARD_LINK;
        }
    }

    void GenericLinkItem::draw(Device* d){

    }

    QRectF GenericLinkItem::boundingRect() const {
        qreal extra = 2;

        return QRectF(node1_qpos, QSizeF(node2_qpos.x() - node1_qpos.y(),
                                         node2_qpos.x() - node1_qpos.y() ))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
    }    

    void GenericLinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        node1_qpos = ((GenericNodeItem*)node1)->pos();
        node2_qpos = ((GenericNodeItem*)node2)->pos();

        node1_rect = ((GenericNodeItem*)node1)->boundingRect();
        node2_rect = ((GenericNodeItem*)node2)->boundingRect();

        center_line = QLineF(node1_qpos, node2_qpos );

        /* Find intersection with first box. */
        QPointF p2;
        QPointF intersection_point_1, intersection_point_2;
        QLineF poly_line[4];

        poly_line[0] = QLineF( node1_rect.topLeft(), node1_rect.bottomLeft());
        poly_line[1] = QLineF( node1_rect.bottomLeft(), node1_rect.bottomRight());
        poly_line[2] = QLineF( node1_rect.bottomRight(), node1_rect.topRight());
        poly_line[3] = QLineF( node1_rect.topRight(), node1_rect.topLeft());

        for( int i = 0; i < 4; i++) {
            poly_line[i].translate(node1_qpos);
            QLineF::IntersectType intersect_type = poly_line[i].intersect(center_line, &intersection_point_1);
            if(intersect_type == QLineF::BoundedIntersection)
                break;
        }

        poly_line[0] = QLineF( node2_rect.topLeft(), node2_rect.bottomLeft());
        poly_line[1] = QLineF( node2_rect.bottomLeft(), node2_rect.bottomRight());
        poly_line[2] = QLineF( node2_rect.bottomRight(), node2_rect.topRight());
        poly_line[3] = QLineF( node2_rect.topRight(), node2_rect.topLeft());

        for( int i = 0; i < 4; i++) {
            poly_line[i].translate(node2_qpos);

            QLineF::IntersectType intersect_type = poly_line[i].intersect(center_line, &intersection_point_2);
            if(intersect_type == QLineF::BoundedIntersection)
                break;
        }

        QLineF line = QLineF(intersection_point_1, intersection_point_2);

        if( link_type == SOFT_LINK )
            painter->setPen(QPen(fill_color, line_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        else if( link_type == HARD_LINK )
            painter->setPen(QPen(fill_color, line_thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter->drawLine(line);

    }

    void GenericLinkItem::updateAttr(Grids::Event *evt) {
        Grids::Object::updateAttr(evt);
    }

    Vec3D GenericLinkItem::getNodePos(Grids::Object* node){
        Vec3D temp_vec = Vec3D();

        /* Check to make sure the pointer is initialized. */
        if(node == 0)
            return temp_vec;

        temp_vec = Vec3D( node->getPosition().X,
                          node->getPosition().Y,
                          node->getPosition().Z );

        return temp_vec;
    }

    Vec3D GenericLinkItem::getNode1Pos() {
        return getNodePos(node1);
    }

    Vec3D GenericLinkItem::getNode2Pos() {
        return getNodePos(node2);
    }

    GenericNodeItem* GenericLinkItem::getNode1() {
        return (GenericNodeItem*)node1;

    }

    GenericNodeItem* GenericLinkItem::getNode2() {
        return (GenericNodeItem*)node2;
    }

    GenericLinkItem::LinkType GenericLinkItem::getLinkType() {
        return link_type;
    }

    void GenericLinkItem::nodeChanged() {
        if (!node1 || !node2)
            return;

        node_changed = true;

        /*
        QLineF line(mapFromItem((QGraphicsItem*)node1, 0, 0),
                    mapFromItem((QGraphicsItem*)node2, 0, 0));
        qreal length = line.length();
        */

        //d->getNoticeWindow()->write(5, tr("Link called"));

        prepareGeometryChange();

        //QLineF line(mapFromItem((QGraphicsItem*)node1, 0, 0), mapFromItem((QGraphicsItem*)node2, 0, 0));
        //qreal length = line.length();

        //prepareGeometryChange();

        //node1_pos = Vec3D(line.p1().x(), line.p1().y(), 0.);
        //node2_pos = Vec3D(line.p2().x(), line.p2().y(), 0.);

                    /*
        if (length > qreal(20.)) {
            QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
            sourcePoint = line.p1() + edgeOffset;
            destPoint = line.p2() - edgeOffset;
        } else {
            sourcePoint = destPoint = line.p1();
        }
        */

        /*
        //node1_pos = node1->getPosition();
        //node2_pos = node2->getPosition();

        node1_pos = Vec3D( ((QGraphicsItem*)node1)->pos().x(),
                           ((QGraphicsItem*)node1)->pos().y(),
                           0.);

        node2_pos = Vec3D( ((QGraphicsItem*)node2)->pos().x(),
                           ((QGraphicsItem*)node2)->pos().y(),
                           0.);

                           */


        //update();
        /*

        if (length > qreal(20.)) {
            QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
            sourcePoint = line.p1() + edgeOffset;
            destPoint = line.p2() - edgeOffset;
        } else {
            sourcePoint = destPoint = line.p1();
        }
        */
    }


}
