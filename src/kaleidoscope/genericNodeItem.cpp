
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QRect>
#include <QGraphicsObject>
#include <QVariant>
#include <QGraphicsPolygonItem>

#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/genericLinkItem.h>
#include <grids/utility.h>
#include <grids/event.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/scene2d.h>
#include <grids/interface.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>

#include <algorithm>


namespace Kaleidoscope {

    GenericNodeItem::GenericNodeItem(Device* d, Grids::Value* val,
                                     QGraphicsItem *parent, QGraphicsScene *scene)
        :  QGraphicsObject(parent), Object(d, val) {
        this->d = d;

        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
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

        /* Set position, set text. */

        setPos( getPosition().X, getPosition().Y);

        std::string attr_text = getTextFromAttr(getAttrFromValue(val));

        text_item = new QGraphicsTextItem(this);
        text_item->setPlainText(tr(attr_text.c_str()));

        /* Set text magenta. */
        text_item->setDefaultTextColor(QColor(text_r, text_g, text_b, text_a));

        text_item->setPos( text_item->pos().x() - text_item->boundingRect().width() / 2,
                           text_item->pos().y() - text_item->boundingRect().height() / 2 );

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

        GenericNodeItem *generic_node = new GenericNodeItem(dev, val);

        scene->addItem(generic_node);
        scene->addNodeItem(generic_node);
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

        QRectF my_rect = boundingRect();
        QRectF other_rect;
        GenericNodeItem* other_node = 0;
        bool intersected = false;

        /* Check to see if it intersected any other object. */
        QList<GenericNodeItem*> node_items =  d->getScene()->getNodeItems();

        foreach(GenericNodeItem* node, node_items){
            other_rect = node->boundingRect();

            if( (this != node) && nodesIntersect(this, node) ){
                intersected = true;
                other_node = node;
                break;
            }
        }

        /* If so, move it back to it's original position. */
        /* Create a new hard link. */

        if(intersected && other_node) {
            setPos(position_change);

            GridsID other_id = d->getObjectController()->getIDFromPointer(other_node);

            GenericLinkItem::requestCreate(d, getID(), other_id, GenericLinkItem::HARD_LINK );
        } else if(pos() != position_change) {
            //d->getNoticeWindow()->write("Pos change");
            updatePosition(d, Vec3D(pos().x(), pos().y(), zValue()));
        }

        update();
        QGraphicsItem::mouseReleaseEvent(event);
    }

    bool GenericNodeItem::nodesIntersect(GenericNodeItem* node1, GenericNodeItem* node2) {
        /*FUCK RECTANGLE INTERSECTION!!!!!!
          WHAT THE FUCK IS THE POINT OF QT, AND ALL THIS OTHER GUI SHIT
          IF IT CAN'T EVEN PERFORM A FUCKING INTERSECTION BETWEEN TWO FUCKING
          RECTANGLES.

          AS A RESULT, I HAVE TO USE THIS PIECE OF SHIT DISTANCE HACK.*/
        /*
        QRectF rect1 = node1->boundingRect();
        QRectF rect2 = node2->boundingRect();
        rect1 = rect1.normalized();
        rect2 = rect2.normalized();



        QPointF tl1 = rect1.topLeft();
        QPointF br1 = rect1.bottomRight();
        QPointF tl2 = rect2.topLeft();
        QPointF br2 = rect2.bottomRight();
        */

        QPointF p1 = node1->pos();
        QPointF p2 = node2->pos();

        Vec3D v1 = Vec3D( (float)(p1.x()), (float)(p1.y()), 0.0 );
        Vec3D v2 = Vec3D( (float)(p2.x()), (float)(p2.y()), 0.0 );

        /*
        return ( ! ( (tl1.x() > br2.x())) || (tl2.x() > br1.x() ) || (tl1.y()
        < br2.y() ) || (tl2.y() < br1.y() ));
        */

        if( v1.getDistanceFrom(v2) < 40.0 )
            return true;
        else
            return false;
    }

    QVariant GenericNodeItem::itemChange(GraphicsItemChange change, const QVariant &value) {
        switch (change) {
        case ItemPositionHasChanged:
            for(int i = 0; i < link_pointers.size(); i++ ){
                ((GenericLinkItem*)(link_pointers[i]))->nodeChanged();
            }
            break;
        default:
            break;
        };

        return QGraphicsItem::itemChange(change, value);
    }

    void GenericNodeItem::calculateForces() {
        if( d->getScene()->mouseGrabberItem() == this) {
            new_pos = pos();
            return;
        }

        // Sum up all forces pushing this item away
        qreal xvel = 0;
        qreal yvel = 0;
        foreach (GenericNodeItem *item, d->getScene()->getNodeItems()) {

            QLineF line(mapFromItem(item, 0, 0), QPointF(0, 0));
            qreal dx = line.dx();
            qreal dy = line.dy();
            double l = 2.0 * (dx * dx + dy * dy);
            if (l > 0) {
                xvel += (dx * 150.0) / l;
                yvel += (dy * 150.0) / l;
            }
        }

        // Now subtract all forces pulling items together
        double weight = (link_pointers.size() + 1) * 10;

        for(int i = 0; i < link_pointers.size(); i++) {
            GenericLinkItem* item = link_pointers[i];

            QPointF pos;
            if (item->getNode1() == this)
                pos = mapFromItem(item->getNode2(), 0, 0);
            else
                pos = mapFromItem(item->getNode1(), 0, 0);

            xvel += pos.x() / weight;
            yvel += pos.y() / weight;
        }

        if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
            xvel = yvel = 0;

        QRectF scene_rect = d->getScene()->sceneRect();
        new_pos = pos() + QPointF(xvel, yvel);
        new_pos.setX(qMin(qMax(new_pos.x(), scene_rect.left() + 10), scene_rect.right() - 10));
        new_pos.setY(qMin(qMax(new_pos.y(), scene_rect.top() + 10), scene_rect.bottom() - 10));

    }

    bool GenericNodeItem::advance() {
        if (new_pos == pos())
            return false;

        setPos(new_pos);
        return true;
    }

    void GenericNodeItem::addLink(GridsID link) {
        addLinkID(link);
    }

    void GenericNodeItem::addLinkID(GridsID link) {
        Grids::Object* obj = d->getObjectController()->getPointerFromID(link);

        if(obj == 0)
            return;

        link_ids.push_back(link);
        addLinkPointer((GenericLinkItem*)obj);
    }

    void GenericNodeItem::addLinkPointer(GenericLinkItem *link) {
        if(link == 0)
            return;

        link_pointers.push_back(link);

        GridsID temp_id = d->getObjectController()->getIDFromPointer(link);

        /* Check to see if the id is in our array. */
        if( std::find(link_ids.begin(), link_ids.end(), temp_id) != link_ids.end() ) {
            link_ids.push_back(temp_id);
        }
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

    /* Is there a more efficient way to do this, so that I don't have to access .x(), y(), etc.
       This function is called every loop. */
    void GenericNodeItem::updateDrawRect(){
        draw_rect = QRectF(text_item->pos().x() + text_item->boundingRect().left() - rect_boarder_width,
                           text_item->pos().y() + text_item->boundingRect().top() - rect_boarder_height,
                           text_item->boundingRect().width() + 2*rect_boarder_width,
                           text_item->boundingRect().height() + 2*rect_boarder_height);
    }





}
