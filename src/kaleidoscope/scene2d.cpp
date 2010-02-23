
#include <QtGui>

#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

namespace Kaleidoscope {

    Scene2D::Scene2D(Device* d, QObject* parent) : QGraphicsScene(parent) {
        this->d = d;
        myTextColor = Qt::black;
    }

    QSize Scene2D::sizeHint() const {
          return QSize(DEFAULT_WINDOW_WIDTH - DEFAULT_SIDEBAR_WIDTH,DEFAULT_WINDOW_HEIGHT);
     }

    void Scene2D::addInputTextItem(Grids::Value* val, Vec3D item_pos) {

    }

    void Scene2D::addGenericNodeItem(Grids::Value *val, Vec3D item_pos) {

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

    /* Allow for a somewhat primative zoom. */
    void Scene2D::wheelEvent(QWheelEvent *event) {
        scaleView(pow((double)2, -event->delta() / 240.0));
    }

    void Scene2D::scaleView(qreal scale_factor) {
        /* TODO: swich display from a QGraphicsScene to a QGraphicsView
        qreal factor = matrix().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scale_factor, scale_factor);
        */
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
