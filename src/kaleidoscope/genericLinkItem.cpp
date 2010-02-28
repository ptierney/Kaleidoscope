
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QRect>
#include <QGraphicsObject>
#include <QGraphicsItem>

#include <kaleidoscope/genericLinkItem.h>
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

        setFlag(ItemIsMovable);

        Grids::Value* attr = getAttrFromValue(val);
        node1_id = getNode1FromAttr(attr);
        node2_id = getNode2FromAttr(attr);

        node1 = d->getObjectController()->getPointerFromID(node1_id);
        node2 = d->getObjectController()->getPointerFromID(node2_id);

        if(node1 == 0 || node2 == 0){
            d->getErrorWindow()->write(7, tr("Error getting node pointers for link: ") + tr(getID().c_str()));
        }

        fill_color_r = 60; fill_color_g = 68; fill_color_b = 132;
        fill_color_a = 255;
        line_thickness = 0.6f;

        node1_pos = getNode1Pos();
        node2_pos = getNode2Pos();
    }

    /* Links node1 (parent) to node2 (child) */
    GridsID GenericLinkItem::requestCreate(Device *dev, GridsID node1, GridsID node2){
        return GenericLinkItem::requestCreate(dev, node1, node2, node1);
    }

    GridsID GenericLinkItem::requestCreate(Device *dev, GridsID req_node1, GridsID req_node2, GridsID parent) {
        Grids::Value* create_val = new Grids::Value();
        GridsID new_id = dev->getGridsUtility()->getNewUUID();

        (*create_val)["type"] = "GenericLink";
        (*create_val)["parent"] = parent;
        (*create_val)["id"] = new_id;
        (*create_val)["node1"] = req_node1;
        (*create_val)["node2"] = req_node2;

        return dev->getInterface()->requestCreateObject(create_val);

        delete create_val;

        return new_id;
    }

    void GenericLinkItem::gridsCreate(Device* dev, Grids::Event* evt) {
        Scene2D* scene = dev->getScene();
        Grids::Value *val = evt->getArgsPtr();

        GenericLinkItem *generic_link = new GenericLinkItem(dev, val);

        scene->addItem(generic_link);
    }

    GridsID GenericLinkItem::getNode1FromAttr(Grids::Value* attr){
        return (*attr)["node1"].asString();
    }

    GridsID GenericLinkItem::getNode2FromAttr(Grids::Value* attr){
        return (*attr)["node2"].asString();
    }

    void GenericLinkItem::draw(Device* d){

    }

    /* TODO: Remove magic numbers. */
    QRectF GenericLinkItem::boundingRect() const {
        return QRectF(  node1_pos.X,
                        node1_pos.Y,
                        node2_pos.X,
                        node2_pos.Y ).normalized();
    }    

    QPainterPath GenericLinkItem::shape() const {
        QPainterPath path;
        path.addRect( boundingRect() );
        //path.a
        return path;
    }

    /* Get these Magic Numbers the fuck out! */
    void GenericLinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        node1_pos = getNode1Pos();
        node2_pos = getNode2Pos();

        QLineF line(QPointF(node1_pos.X, node1_pos.Y),
                    QPointF(node2_pos.X, node2_pos.Y) );

        //painter->setBrush(QColor(fill_color_r, fill_color_g, fill_color_b, fill_color_a));
        //painter->setPen(QPen(Qt::black, line_thickness));
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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


}
