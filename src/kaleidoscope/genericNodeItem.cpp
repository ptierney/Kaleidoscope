
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

        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        setAcceptHoverEvents(true);


        line_thickness = 0.6f;
        fill_color_r = 145; fill_color_g = 145; fill_color_b = 145;
        non_active_r = 145; non_active_g = 145; non_active_b = 145;
        fill_color_a = 255;
        non_active_a = 190;

        current_r = non_active_r;
        current_g = non_active_g;
        current_b = non_active_b;
        current_a = non_active_a;

        text_r = 0; text_g = 0; text_b = 0;
        text_a = 255;

        rect_radius_width = 7.0;
        rect_radius_height = 5.0;
        rect_boarder_width = 8.0;
        rect_boarder_height = 1.0;

        d->getNoticeWindow()->write(QObject::tr("Creating generic node"));

        std::string attr_text = getTextFromAttr(getAttrFromValue(val));

        text_item = new QGraphicsTextItem(this);
        text_item->setPlainText(tr(attr_text.c_str()));

        /* Set text magenta. */
        text_item->setDefaultTextColor(QColor(text_r, text_g, text_b, text_a));

        updateDrawRect();
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

    QRectF GenericNodeItem::boundingRect() const {
        qreal adjust = 2;
        return draw_rect.adjusted(-adjust, -adjust, adjust, adjust).normalized();
    }

    QPainterPath GenericNodeItem::shape() const {
        QPainterPath path;
        path.addRect(draw_rect);
        return path;
    }

    void GenericNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        //d->getNoticeWindow()->write("Paint");
        /*float x1 = text_item->boundingRect().left() - rect_boarder;
        float y1 = text_item->boundingRect().top() - rect_boarder;
        float x2 = text_item->boundingRect().right() + rect_boarder;
        float y2 = text_item->boundingRect().bottom() + rect_boarder;
        */
        updateDrawRect();

        painter->setBrush(QColor(current_r, current_g, current_b, current_a));
        painter->setPen(QPen(QColor(0, 0, 0, current_a), line_thickness));
        //painter->drawRoundedRect(QRect());
        painter->drawRoundedRect(draw_rect, rect_radius_width, rect_radius_height);
    }

    void GenericNodeItem::setLocalPosition(Vec3D vec) {
        setPos(vec.X, vec.Y);
        Grids::Object::setLocalPosition(vec);
    }

    void GenericNodeItem::updateAttr(Grids::Event *evt) {
        Grids::Object::updateAttr(evt);
    }

    void GenericNodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        position_change = pos();

        update();
        QGraphicsItem::mousePressEvent(event);
    }


    void GenericNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        if(pos() != position_change) {
            d->getNoticeWindow()->write("Pos change");
            updatePosition(d, Vec3D(pos().x(), pos().y(), zValue()));
        }

        update();
        QGraphicsItem::mouseReleaseEvent(event);

    }

    void GenericNodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
        //d->getNoticeWindow()->write(6, tr("hover"));
        current_a = fill_color_a;
        current_r = fill_color_r;
        current_g = fill_color_g;
        current_b = fill_color_b;

        update();

        setCursor(Qt::ArrowCursor);

        QGraphicsItem::hoverEnterEvent(event);

    }

    void GenericNodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){        
        current_r = non_active_r;
        current_g = non_active_g;
        current_b = non_active_b;
        current_a = non_active_a;

        update();
        setCursor(Qt::OpenHandCursor);

        QGraphicsItem::hoverLeaveEvent(event);
    }

    void GenericNodeItem::updateDrawRect(){
        draw_rect = QRectF(text_item->boundingRect().left() - rect_boarder_width,
                           text_item->boundingRect().top() - rect_boarder_height,
                           text_item->boundingRect().width() + 2*rect_boarder_width,
                           text_item->boundingRect().height() + 2*rect_boarder_height);
    }





}
