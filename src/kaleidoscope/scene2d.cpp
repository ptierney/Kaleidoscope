
#include <QtGui>

#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/genericNodeItem.h>
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

         startTimer(100);
    }

    QSize Scene2D::sizeHint() const {
          return QSize(DEFAULT_WINDOW_WIDTH - DEFAULT_SIDEBAR_WIDTH,DEFAULT_WINDOW_HEIGHT);
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


    void Scene2D::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        if(event->button() == Qt::RightButton)
            InputTextItem::requestCreate(d, Vec3D( event->scenePos().x(),
                                               event->scenePos().y(),
                                               1000.0) );

        if(event->button() == Qt::LeftButton)
            down_pos = event->scenePos();


        QGraphicsScene::mousePressEvent(event);
    }

    void Scene2D::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsScene::mouseMoveEvent(event);
    }

    void Scene2D::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        QGraphicsScene::mouseReleaseEvent(event);

        if(event->button() == Qt::LeftButton &&
           focusItem() == 0 &&
           selectedItems().length() == 0 &&
           down_pos == event->scenePos() ){
            InputTextItem::requestCreate(d,Vec3D(event->scenePos().x(), event->scenePos().y(), 1000.0));
        }
    }

    void Scene2D::keyPressEvent(QKeyEvent *event) {

        if(focusItem() == 0)
            InputTextItem::requestCreate(d,Vec3D(qrand() % 1000 - 500,
                                                 qrand() % 1000 - 500,
                                                 1000.0));
        QGraphicsScene::keyPressEvent(event);
    }


    /* Allow for a somewhat primative zoom. */
    void Scene2D::wheelEvent(QWheelEvent *event) {
        scaleView(pow((double)2, -event->delta() / 240.0));
    }

    void Scene2D::scaleView(qreal /*scale_factor*/) {
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

    void Scene2D::addInputTextItem(InputTextItem* item) {
        input_text_items.append(item);
    }

    void Scene2D::addNodeItem(GenericNodeItem *item) {
        node_items.append(item);
    }

    void Scene2D::addLinkItem(GenericLinkItem *item) {
        link_items.append(item);
    }

    QList<InputTextItem*> Scene2D::getInputTextItems() {
        return input_text_items;
    }

    QList<GenericNodeItem*> Scene2D::getNodeItems() {
        return node_items;
    }

    QList<GenericLinkItem*> Scene2D::getLinkItems() {
        return link_items;
    }

    void Scene2D::timerEvent(QTimerEvent *event) {
        Q_UNUSED(event);

        /* Don't caluculate while dragging. */
        if(mouseGrabberItem() )
            return;

        foreach(GenericNodeItem* item, node_items) {
            item->calculateForces();
        }

        foreach(GenericNodeItem* item, node_items) {
            item->advance();
        }
    }

    View2D* Scene2D::main_view(){
      return main_view_;
    }

    void Scene2D::set_main_view(View2D* main_view){
      main_view_ = main_view;
    }

}
