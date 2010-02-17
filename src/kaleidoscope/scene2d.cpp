
#include <QtGui>

#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/noticeWindow.h>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

namespace Kaleidoscope {

    Scene2D::Scene2D(Device* d, QObject* parent) : QGraphicsScene(parent) {  
        this->d = d;
        myTextColor = Qt::black;
    }

    void Scene2D::addInputTextItem(Grids::Value* val, Vec3D item_pos) {

        textItem = new InputTextItem(d, val);
        /*textItem->setFont(myFont);*/
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        /* Set the stack order.  This should be adjusted. */
        textItem->setZValue(item_pos.Z);
        connect(textItem, SIGNAL(lostFocus(InputTextItem*)),
                this, SLOT(editorLostFocus(InputTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        /*textItem->setPos(mouseEvent->scenePos());*/
        textItem->setPos(QPointF(item_pos.X, item_pos.Y));
        textItem->setPlainText(tr("This is a test"));
        /*emit textInserted(textItem);*/
    }

    void Scene2D::editorLostFocus(InputTextItem *item) {
        QTextCursor cursor = item->textCursor();
        cursor.clearSelection();
        item->setTextCursor(cursor);

        if (item->toPlainText().isEmpty()) {
            d->getNoticeWindow()->write(tr("Deleting empty item."));
            removeItem(item);
            item->deleteLater();
        }
    }


    void Scene2D::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {


        if(mouseEvent->button() == Qt::RightButton)
            InputTextItem::requestCreate(d, Vec3D( mouseEvent->scenePos().x(),
                                               mouseEvent->scenePos().y(),
                                               1000.0) );
        /*
        if (mouseEvent->button() != Qt::LeftButton)
            return;
            */

        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    void Scene2D::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

    void Scene2D::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }

    void Scene2D::setFont(const QFont &font) {
        myFont = font;

        QGraphicsTextItem *item =
                qgraphicsitem_cast<InputTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);

    }

    void Scene2D::setTextColor(const QColor &color) {
        myTextColor = color;
        InputTextItem *item =
                qgraphicsitem_cast<InputTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }






}
