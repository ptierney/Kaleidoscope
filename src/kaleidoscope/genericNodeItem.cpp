


#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <grids/utility.h>
#include <grids/interface.h>

namespace Kaleidoscope {

    GenericNodeItem::GenericNodeItem(Device* d, Grids::Value* val,
                                     QGraphicsItem *parent, QGraphicsScene *scene)
        :  QGraphicsItem(parent, scene), Object(d, val) {
        this->d = d;
        setFlag(ItemIsMovable);

        d->getNoticeWindow()->write(QObject::tr("Creating generic node"));
    }

    void GenericNodeItem::draw(Device* d) {

    }

    GridsID GenericNodeItem::requestCreate(Device *dev, Vec3D position){
        Grids::Value* create_val = new Grids::Value();

        (*create_val)["type"] = "GenericNode";
        (*create_val)["parent"] = dev->getMyID();
        (*create_val)["id"] = dev->getGridsUtility()->getNewUUID();

        return dev->getInterface()->requestCreateObject(create_val, position);

        delete create_val;
    }

    void GenericNodeItem::gridsCreate(Device* dev, Grids::Event *evt) {
        dev->getNoticeWindow()->write(QObject::tr("Creating generic node"));
        //dev->getScene()->addGenericNodeItem(evt->getArgsPtr(), evt->getPosition());

        Scene2D* scene = dev->getScene();
        Grids::Value *val = evt->getArgsPtr();
        Vec3D item_pos = evt->getPosition();

        GenericNodeItem *generic_node = new GenericNodeItem(dev, val);

        scene->addItem(generic_node);
        generic_node->setPos(item_pos.X, item_pos.Y);
    }

    /* TODO: Remove magic numbers. */
    QRectF GenericNodeItem::boundingRect() const {
        return QRectF(BOUNDMIN, BOUNDMIN,
                      BOUNDMAX, BOUNDMAX);
    }

    QPainterPath GenericNodeItem::shape() const {
        QPainterPath path;
        path.addEllipse(-10, -10, 20, 20);
        return path;
    }

    void GenericNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        //d->getNoticeWindow()->write("Paint");
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse(-7, -7, 20, 20);

        QRadialGradient gradient(-3, -3, 10);
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            gradient.setColorAt(1, QColor(Qt::yellow).light(120));
            gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
        } else {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::darkYellow);
        }

        painter->setBrush(gradient);
        painter->setPen(QPen(Qt::black, 0));
        painter->drawEllipse(-10, -10, 20, 20);
    }

    void GenericNodeItem::setLocalPosition(Vec3D vec) {
        setPos(vec.X, vec.Y);
        Grids::Object::setLocalPosition(vec);
    }

    void GenericNodeItem::updateAttr(Grids::Event *evt) {

    }


}
