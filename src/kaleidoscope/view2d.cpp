
#include <math.h>

#include <QGraphicsScene>
#include <QWheelEvent>

#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  View2D::View2D(Device* d, Scene2D *scene)
    : QGraphicsView(scene) {
    d_ = d;

    /* "Use this if your scene has many moving elements." */
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    setScene(scene);
    scene->set_main_view(this);

    setCacheMode(CacheBackground);
    //setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    setDragMode( QGraphicsView::ScrollHandDrag);

    scale(qreal(1.2), qreal(1.2));
    setMinimumSize(400, 400);
    //centerOn(10000.0, 10000.0);
  }

  void View2D::drawBackground(QPainter* /*painter*/, const QRectF &rect)
  {
    Q_UNUSED(rect);
    /*
        QRectF sceneRect = this->sceneRect();

        // Fill
        QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
        gradient.setColorAt(0, QColor(0, 0, 255, 50) );
        gradient.setColorAt(1, QColor(0, 0, 0, 50) );
        painter->fillRect(rect.intersect(sceneRect), gradient);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(sceneRect);
        */
  }

  void View2D::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, event->delta() / 240.0));
  }

  void View2D::keyPressEvent(QKeyEvent* event){
    //d_->event_controller()->keyPressEvent(event);
    QGraphicsView::keyPressEvent(event);
  }

  /* Scales the view by the fraction passed in.
       0.5 makes everything half as big. */
  void View2D::scaleView(qreal scale_factor) {
    //qreal scale_factor_ = matrix().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
    //if (factor < 0.07 || factor > 100)
    //    return;

    scale(scale_factor, scale_factor);
  }

}
