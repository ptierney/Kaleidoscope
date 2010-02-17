
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

        setCursor(Qt::IBeamCursor);

        /* If I created this item, select it. */
        if(d->myChild(getParentID())){
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

    QVariant InputTextItem::itemChange(GraphicsItemChange change,
                         const QVariant &value)
    {
        if (change == QGraphicsItem::ItemSelectedHasChanged)
            emit selectedChange(this);
        return value;
    }

    void InputTextItem::focusOutEvent(QFocusEvent *event)
    {
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

    /* Required function */
    void InputTextItem::draw( Device* d ){

    }


}
