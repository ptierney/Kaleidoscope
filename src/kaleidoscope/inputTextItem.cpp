
#include <QtGui>

#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/object.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/noticeWindow.h>
#include <grids/utility.h>
#include <grids/interface.h>


namespace Kaleidoscope {

    InputTextItem::InputTextItem(Device* d, Grids::Value* val,
                                 QGraphicsItem *parent, QGraphicsScene *scene ) :
    QGraphicsTextItem(parent, scene), Object(d, val) {

        this->d = d;

        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsFocusable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

        /* A segfault keeps on happening here. */
        //setCursor(Qt::IBeamCursor);

        /* If I created this item, select it and add a cursor. */
        if(d->myChild(getParentID())){
            QMutexLocker locker(&focus_mutex);
            setSelected(1);
            setFocus(Qt::OtherFocusReason);
        }
    }

    GridsID InputTextItem::requestCreate( Device* device, Vec3D start_pos) {
        Grids::Value* create_val = new Grids::Value();

        (*create_val)["type"] = "InputText";
        (*create_val)["parent"] = device->getMyID();
        (*create_val)["id"] = device->getGridsUtility()->getNewUUID();

        return device->getInterface()->requestCreateObject( create_val, start_pos);
    }

    void InputTextItem::gridsCreate(Device *dev, Grids::Event *evt) {
        dev->getNoticeWindow()->write(tr("Creating input text"));
        dev->getScene()->addInputTextItem(evt->getArgsPtr(), evt->getPosition());
   }

    /* Part of the object has changed.  Broadcast a message to Grids that lets
       the server know of the change. */
    QVariant InputTextItem::itemChange(GraphicsItemChange change,
                         const QVariant &value)
    {
        if (change == QGraphicsItem::ItemSelectedHasChanged)
            emit selectedChange(this);

        /* Check for text change. */

        /*d->getNoticeWindow()->write("change");*/

        /* Check for position change. */
        if(change == QGraphicsItem::ItemPositionChange) {
            d->getNoticeWindow()->write("Pos change");
            updatePosition(d, Vec3D(value.toPointF().x(), value.toPointF().y(), zValue()));
        }
        return value;
        //return QGraphicsItem::itemChange(change, value);
    }

    void InputTextItem::setLocalPosition(Vec3D new_pos){
        setPos(new_pos.X, new_pos.Y);
    }

    void InputTextItem::focusInEvent(QFocusEvent* event) {
        QMutexLocker locker(&focus_mutex);
        d->getNoticeWindow()->write(tr("focus"));
    }

    void InputTextItem::focusOutEvent(QFocusEvent *event) {
        QMutexLocker locker(&focus_mutex);
        d->getNoticeWindow()->write(tr("out focus"));

        setTextInteractionFlags(Qt::NoTextInteraction);
        emit lostFocus(this);
        QGraphicsTextItem::focusOutEvent(event);
    }

    void InputTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        if (textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }


    void InputTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
        position_change = pos();
        QGraphicsTextItem::mousePressEvent(event);
    }

    void InputTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        if(pos() != position_change) {
            d->getNoticeWindow()->write("Pos change");
            updatePosition(d, Vec3D(pos().x(), pos().y(), zValue()));
        }
        QGraphicsTextItem::mouseReleaseEvent(event);
    }

    /* Required function */
    void InputTextItem::draw( Device* d ){

    }


}
