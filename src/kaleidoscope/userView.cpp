
#include <QGraphicsView>
#include <QGLFormat>
#include <QGLWidget>

#include <kaleidoscope/userView.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {

  UserView::UserView(Device* d, User* user, Scene2D* scene) :
      QGraphicsView(scene) {
    d_ = d;
    user_ = user;

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
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setDragMode( QGraphicsView::ScrollHandDrag);
  }

  UserView::~UserView(){

  }

  void UserView::init(){

  }

  User* UserView::user(){
    return user_;
  }

  void UserView::digestTete(Tete* tete){
    if(tete->tete_node() == NULL)
      return;

    ensureVisible(tete->tete_node()->boundingRect());
  }

  // Highlight the focused node
  void UserView::mousePressEvent(QMouseEvent* event){

  }

  // Zoom in on the focused node
  void UserView::mouseDoubleClickEvent(QMouseEvent* event){

  }

}
