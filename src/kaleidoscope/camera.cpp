
#include <kaleidoscope/camera.h>
#include <grids/gObject.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>
#include <grids/irrMath.h>

#include <iostream>
#include <QtOpenGL>
#include <QtGlobal>


namespace Kaleidoscope {

	Camera::Camera(Device* d, Grids::Value* val, QWidget* parent) 
                :  QWidget(parent), Object(d, val) {
		this->d = d;

		d->getNoticeWindow()->addNotice(0, tr("Creating camera"));

		parseAttrFromValue(val);
		mouse_pressed = false;
	}


        /* This is the main loop of the program, similar to Processing's
         * draw() function. */
  void Camera::paintEvent(QPaintEvent* /*event*/) {
                /* Makes this context the current OpenGL rendering context.
		makeCurrent();
                */
                /*
		d->getRenderer()->prepareRender1();
		d->getRenderer()->prepareRender2();
                */

                //d->getRenderer()->drawAll(d);
	}
	
	void Camera::parseAttrFromValue( Grids::Value* val ){
		parseAttr( getAttrFromValue( val ) );
	}
	
	void Camera::parseAttr( Grids::Value* val ){
                /* Position automatically captured by base class. */
            /* I suppose the target is the mouse location atm. */
		Vec3D temp_target = Vec3D( (*val)[ "target" ][ 0u ].asDouble(), 
                                           (*val)[ "target" ][ 1u ].asDouble(),
                                        (*val)[ "target" ][ 2u ].asDouble() ).normalize();

		setTarget( temp_target );
	}

  void Camera::keyPressEvent(QKeyEvent* /*event*/) {
                /*if(event->key() == Qt::Key_Space)
                  perhaps switch between navigation modes.
                */
        }
	
	void Camera::mouseMoveEvent(QMouseEvent* event) {
		doMovement(NULL, event, NULL);
	}

	void Camera::mousePressEvent(QMouseEvent* event) {
		mouse_pressed = true;
		doMovement(NULL, event, NULL);
	}

	void Camera::mouseReleaseEvent(QMouseEvent* event) {
		mouse_pressed = false;
		doMovement(NULL, event, NULL);
	}
	
  void Camera::wheelEvent(QWheelEvent* event) {
		doMovement(NULL, NULL, event);
	}

        void Camera::mouseMoved(QMouseEvent* /*event*/){

        }

        void Camera::keyboardPressed(QKeyEvent* /*event*/) {

        }

        void Camera::keyboardReleased(QKeyEvent* /*event*/) {

        }

  void Camera::doMovement(QKeyEvent* /*k_event*/, QMouseEvent* /*m_event*/, QWheelEvent* /*w_event*/) {

	}

        /* A public slot, probably to be used to display other people's views. */
        void Camera::updatePosition(Vec3D /*new_pos*/, Vec3D /*new_rot*/, Vec3D /*new_tar*/) {

        }

        /* Required functions */
        bool Camera::detectSelection() { return false; }
        void Camera::select() { }
        void Camera::deselect() { }

  void Camera::draw( Device* /*d*/ ){

	}
	
        /********************
         * Accessor functions
         */
	void Camera::setTarget( Vec3D vec ){ 
                //QMutexLocker lock(&target_mutex);
		target = vec;
	}
	
	Vec3D Camera::getTarget(){
                //QMutexLocker lock(&target_mutex);
		return target;
	}

} // end namespace Kaleidoscope
