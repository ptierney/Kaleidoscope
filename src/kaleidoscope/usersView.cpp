
#include <iostream>

#include <QGLFormat>
#include <QWheelEvent>

#include <kaleidoscope/usersView.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UsersView::UsersView(Device* d, UsersScene* scene) :
      QGraphicsView(scene) {
    d_ = d;
    //setMouseTracking(true);

    if(KALEIDOSCOPE_USE_OPENGL){
      QGLFormat format;
      format.setSampleBuffers(true);

      // This is how you force the View to draw everything in OpenGL
      QGLWidget* gl_widget = new QGLWidget(format);
      setViewport(gl_widget);
    } else {
      setRenderHint(QPainter::Antialiasing);
    }

    setScene(scene);
    scene->set_users_view(this);

    setDragMode( QGraphicsView::ScrollHandDrag);
    setCacheMode(CacheBackground);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    QMatrix temp_matrix;
    temp_matrix.scale(0.75, 0.75);
    setMatrix(temp_matrix);

    setMinimumSize(200, 200);
  }

  UsersView::~UsersView(){

  }

  void UsersView::init(){
   // Hella more shit should go in here not in the con
  }

  void UsersView::drawBackground(QPainter* painter, const QRectF &rect) {
    painter->setBrush(QBrush(Qt::black));
    //painter->drawRect(rect);
  }

  void UsersView::wheelEvent(QWheelEvent* event) {
    scaleView(pow((double)2, event->delta() / 240.0));
  }

  void UsersView::scaleView(qreal scale_factor) {
    scale(scale_factor, scale_factor);
  }

}
