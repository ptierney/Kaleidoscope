
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/device.h>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

namespace Kaleidoscope {

    Scene2D::Scene2D(Device* d, QObject* parent) : QGraphicsScene(parent) {
        this->d = d;
    }

    void Scene2D::editorLostFocus(InputTextItem *item) {
        QTextCursor cursor = item->textCursor();
        cursor.clearSelection();
        item->setTextCursor(cursor);

        if (item->toPlainText().isEmpty()) {
            removeItem(item);
            item->deleteLater();
        }
    }

    void Scene2D::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        if (mouseEvent->button() != Qt::LeftButton)
            return;

        InputTextItem::requestCreate(d, Vec3D( mouseEvent->scenePos().x(),
                                               mouseEvent->scenePos().y(),
                                               1000.0) );

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    void Scene2D::addInputTextItem(Grids::Value* val, Vec3D item_pos) {

        textItem = new InputTextItem(d, val);
        /*textItem->setFont(myFont);*/
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        /* Set the stack order.  This should be adjusted. */
        textItem->setZValue(item_pos.Z);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        /*textItem->setDefaultTextColor(myTextColor);*/
        /*textItem->setPos(mouseEvent->scenePos());*/
        textItem->setPos(QPointF(item_pos.X, item_pos.Y));
        /*emit textInserted(textItem);*/
    }

    void Scene2D::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

    void Scene2D::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }

    /*
    void Scene2D::setFont(const QFont &font) {
        myFont = font;

        if (isItemChange(InputTextItem::Type)) {
            QGraphicsTextItem *item =
                qgraphicsitem_cast<InputTextItem *>(selectedItems().first());
            //At this point the selection can change so the first selected item might not be a DiagramTextItem
            if (item)
                item->setFont(myFont);
        }
    }
    */

}
