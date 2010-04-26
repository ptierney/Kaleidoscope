
#include <math.h>

#include <iostream>

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGLWidget>
#include <QPrinter>
#include <QPrintDialog>

#include <kaleidoscope/define.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/linkCreator.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  View2D::View2D(Device* d, Scene2D* scene)
    : QGraphicsView(scene) {
    d_ = d;
    setMouseTracking(true);
    //setDragEnabled(true);

    if(KALEIDOSCOPE_USE_OPENGL){
      QGLFormat format;
      format.setSampleBuffers(true);

      // This is how you force the View to draw everything in OpenGL
      QGLWidget* gl_widget = new QGLWidget(format);
      setViewport(gl_widget);
    } else {
      setRenderHint(QPainter::Antialiasing);
    }

    /* "Use this if your scene has many moving elements." */
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    setScene(scene);
    scene->set_main_view(this);

    setCacheMode(CacheBackground);
    //setViewportUpdateMode(BoundingRectViewportUpdate);
    //setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    setDragMode( QGraphicsView::ScrollHandDrag);

    //scale(qreal(0.5), qreal(0.5));
    QMatrix temp_matrix;
    temp_matrix.scale(1.5, 1.5);
    setMatrix(temp_matrix);

    setMinimumSize(600, 400);
    //centerOn(10000.0, 10000.0);

    link_creator_ = new LinkCreator(d_);
    scene->addItem(link_creator_);
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

  void View2D::focusOutEvent(QFocusEvent* /*event*/){
    //std::cerr << "Out" << std::endl;
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

  void View2D::mousePressEvent(QMouseEvent *event) {
    d_->chat_controller()->stopZooming();
    QGraphicsView::mousePressEvent(event);
  }

  void View2D::mouseReleaseEvent(QMouseEvent *event) {
     d_->chat_controller()->startZooming();
     QGraphicsView::mouseReleaseEvent(event);
  }

  void View2D::print(){
    QPrinter printer;
    if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        render(&painter);
    }
  }

  LinkCreator* View2D::link_creator(){
    return link_creator_;
  }

  /*
  void View2D::render(QPainter* painter, const QRectF& target, const QRect& source, Qt::AspectRatioMode aspect_ratio_mode) {


    QGraphicsView::render(painter, target, source, aspect_ratio_mode);
  }
  */

}
