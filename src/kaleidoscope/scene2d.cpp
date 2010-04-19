
#include <math.h>

#include <QtGui>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/device.h>

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
      //d->getNoticeWindow()->write(tr("Deleting empty item."));
      removeItem(item);
      item->deleteLater();
    }
  }

  void Scene2D::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton)
      down_pos = event->scenePos();

    QGraphicsScene::mousePressEvent(event);
  }



  void Scene2D::keyPressEvent(QKeyEvent *event) {
    // If we are not in a text box
    if(focusItem() == 0){
      d->event_controller()->keyPressEvent(event);
    }

    QGraphicsScene::keyPressEvent(event);
  }


  /* Allow for a somewhat primative zoom. */
  void Scene2D::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
  }

  void Scene2D::scaleView(qreal /*scale_factor*/) {

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

  const QList<InputTextItem*>& Scene2D::getInputTextItems() {
    return input_text_items;
  }

  const QList<GenericNodeItem*>& Scene2D::getNodeItems() {
    return node_items;
  }

  const QList<GenericLinkItem*>& Scene2D::getLinkItems() {
    return link_items;
  }

  void Scene2D::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event);

    /* Don't caluculate while dragging. */
    if(mouseGrabberItem() )
      return;

    /*
    foreach(GenericNodeItem* item, node_items) {
      item->calculateForces();
    }

    foreach(GenericNodeItem* item, node_items) {
      item->advance();
    }
    */
  }

  View2D* Scene2D::main_view(){
    return main_view_;
  }

  void Scene2D::set_main_view(View2D* main_view){
    main_view_ = main_view;
  }

}
