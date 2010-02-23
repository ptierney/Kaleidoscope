

#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/define.h>
#include <grids/utility.h>
#include <grids/event.h>
#include <kaleidoscope/object.h>
#include <kaleidoscope/scene2d.h>
#include <grids/interface.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

#include <QtGui>

namespace Kaleidoscope {

    InputTextItem::InputTextItem(Device* d, Grids::Value* val,
                                 QGraphicsItem *parent, QGraphicsScene *scene ) :
    QGraphicsTextItem(parent, scene), Object(d, val) {

        this->d = d;

        d->getNoticeWindow()->write(0, tr("Creating InputTextItem"));

        text_color = QColor( (*(getAttrFromValue(val)))["text_color"][0u].asInt(),
                             (*(getAttrFromValue(val)))["text_color"][1u].asInt(),
                             (*(getAttrFromValue(val)))["text_color"][2u].asInt(),
                             (*(getAttrFromValue(val)))["text_color"][3u].asInt() );


        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsFocusable);
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

        setCursor(Qt::IBeamCursor);

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
        (*create_val)["text_color"][0u] = qrand() % 256;
        (*create_val)["text_color"][1u] = qrand() % 256;
        (*create_val)["text_color"][2u] = qrand() % 256;
        (*create_val)["text_color"][3u] = 255;

        (*create_val)["id"] = device->getGridsUtility()->getNewUUID();

        return device->getInterface()->requestCreateObject( create_val, start_pos);

        delete create_val;
    }

    void InputTextItem::gridsCreate(Device *dev, Grids::Event *evt) {
        dev->getNoticeWindow()->write(tr("Creating input text"));
        /*dev->getScene()->addInputTextItem(evt->getArgsPtr(), evt->getPosition()); */

        Scene2D *scene = dev->getScene();
        Grids::Value *val = evt->getArgsPtr();
        Vec3D item_pos = evt->getPosition();

        InputTextItem *text_item = new InputTextItem(dev, val);
        /*text_item->setFont(myFont);*/
        text_item->setTextInteractionFlags(Qt::TextEditorInteraction);
        /* Set the stack order.  This should be adjusted. */
        text_item->setZValue(item_pos.Z);
        connect(text_item, SIGNAL(lostFocus(InputTextItem*)),
                scene, SLOT(editorLostFocus(InputTextItem*)));
        connect(text_item, SIGNAL(selectedChange(QGraphicsItem*)),
                scene, SIGNAL(itemSelected(QGraphicsItem*)));
        scene->addItem(text_item);
        text_item->setDefaultTextColor(text_item->getTextColor() );

        /*text_item->setPos(mouseEvent->scenePos());*/
        text_item->setPos(QPointF(item_pos.X, item_pos.Y));
        //text_item->setPlainText(tr("This is a test"));
        /*emit textInserted(textItem);*/
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

    /* Overload Grids::Object */
    void InputTextItem::setLocalPosition(Vec3D new_pos){
        setPos(new_pos.X, new_pos.Y);
        Grids::Object::setLocalPosition(new_pos);
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

        if(hasFocus()){
            if (textInteractionFlags() == Qt::NoTextInteraction)
                setTextInteractionFlags(Qt::TextEditorInteraction);
        }

        QGraphicsTextItem::mousePressEvent(event);
    }

    void InputTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        if(pos() != position_change) {
            d->getNoticeWindow()->write("Pos change");
            updatePosition(d, Vec3D(pos().x(), pos().y(), zValue()));
        }
        QGraphicsTextItem::mouseReleaseEvent(event);
    }

    void InputTextItem::keyPressEvent(QKeyEvent* event) {
        /* Process the event so that toPlainText() returns the actual value of the item. */
        QGraphicsTextItem::keyPressEvent(event);

        if(hasFocus()){
            /*d->getNoticeWindow()->write(event->text());*/
            updateText(toPlainText());
        }   
    }

    void InputTextItem::updateText(QString new_text) {
        Grids::Value *temp_attr = new Grids::Value();

        (*temp_attr)["text"] = new_text.toStdString();
        (*temp_attr)["writer"] = d->getMyID();

        d->getInterface()->printVal(temp_attr);

        d->getInterface()->requestUpdateAttr(getID(), temp_attr);

        delete temp_attr;
    }

    void InputTextItem::updateAttr(Grids::Event *evt) {
        
        if((*(evt->getArgsPtr()))["attr"].empty()){
            d->getNoticeWindow()->write(tr("Received empty update attr"));
            return;
        }

        GridsID writer_id = (*(evt->getArgsPtr()))["attr"]["writer"].asString();

        /* If the update was made by someone else, update the text. */
        if(d->getMyID() != writer_id ) {
            std::string new_args_text = (*(evt->getArgsPtr()))["attr"]["text"].asString();
            QString new_text = tr(new_args_text.c_str());
            /*d->getNoticeWindow()->write(tr("new text>> ") + new_text);*/

            setPlainText(new_text);
        }
    }

    QColor InputTextItem::getTextColor() {
        return text_color;
    }


    /* Required function */
    void InputTextItem::draw( Device* d ){

    }


}
