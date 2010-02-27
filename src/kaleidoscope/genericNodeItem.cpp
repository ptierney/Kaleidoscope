
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QRect>
#include <QGraphicsObject>

#include <kaleidoscope/genericNodeItem.h>
#include <grids/utility.h>
#include <grids/event.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <grids/interface.h>
#include <kaleidoscope/device.h>


namespace Kaleidoscope {

    GenericNodeItem::GenericNodeItem(Device* d, Grids::Value* val,
                                     QGraphicsItem *parent, QGraphicsScene *scene)
        :  QGraphicsObject(parent), Object(d, val) {
        this->d = d;
        setFlag(ItemIsMovable);
        rect_boarder = 1.f;
        line_thickness = 0.6f;
        fill_color_r = 60; fill_color_g = 68; fill_color_b = 132;
        text_r = 226; text_g = 224; text_b = 211;
        fill_color_a = 255;
        text_a = 255;

        d->getNoticeWindow()->write(QObject::tr("Creating generic node"));

        /* From the docs:
           Note that setting QSvgRenderer on a QGraphicsSvgItem doesn't make the item take
           ownership of the renderer, therefore if using setSharedRenderer() method one has
           to make sure that the lifetime of the QSvgRenderer object will be at least as
           long as that of the QGraphicsSvgItem.
           */
        /* TODO: find a way to delete renderer, move it to the descructor? */
        /*
        QSvgRenderer *renderer = new QSvgRenderer(QLatin1String("../../media/rect.svg"));
        svg_item = new QGraphicsSvgItem(this);
        svg_item->setSharedRenderer(renderer);
        */

        std::string attr_text = getTextFromAttr(getAttrFromValue(val));

        text_item = new QGraphicsTextItem(this);
        text_item->setPlainText(tr(attr_text.c_str()));

        /* Set text magenta. */
        text_item->setDefaultTextColor(QColor(text_r, text_g, text_b, text_a));
    }

    std::string GenericNodeItem::getTextFromAttr(Grids::Value* attr){
        return (*attr)["text"].asString();
    }

    void GenericNodeItem::draw(Device* d) {

    }

    GridsID GenericNodeItem::requestCreate(Device *dev, Vec3D position, std::string text){
        Grids::Value* create_val = new Grids::Value();
        GridsID new_id = dev->getGridsUtility()->getNewUUID();

        (*create_val)["type"] = "GenericNode";
        (*create_val)["parent"] = dev->getMyID();
        (*create_val)["id"] = new_id;
        (*create_val)["text"] = text;

        return dev->getInterface()->requestCreateObject(create_val, position);

        delete create_val;

        return new_id;
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
        return text_item->boundingRect();
    }

    QPainterPath GenericNodeItem::shape() const {
        QPainterPath path;
        path.addEllipse(-10, -10, 20, 20);
        return path;
    }

    /* Get these Magic Numbers the fuck out! */
    void GenericNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        //d->getNoticeWindow()->write("Paint");
        float x1 = text_item->boundingRect().left() - rect_boarder;
        float y1 = text_item->boundingRect().top() - rect_boarder;
        float x2 = text_item->boundingRect().right() + rect_boarder;
        float y2 = text_item->boundingRect().bottom() + rect_boarder;

        painter->setBrush(QColor(fill_color_r, fill_color_g, fill_color_b, fill_color_a));
        painter->setPen(QPen(Qt::black, line_thickness));
        painter->drawRect(x1, y1, x2, y2);
    }

    void GenericNodeItem::setLocalPosition(Vec3D vec) {
        setPos(vec.X, vec.Y);
        Grids::Object::setLocalPosition(vec);
    }

    void GenericNodeItem::updateAttr(Grids::Event *evt) {
        Grids::Object::updateAttr(evt);
    }


}
