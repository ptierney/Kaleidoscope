
#pragma once

#include <kaleidoscope/define.h>
#include <grids/define.h>
#include <kaleidoscope/object.h>
#include <QGLWidget>
#include <QTimer>

namespace Kaleidoscope {
  class CursorController;

  class Camera : public QWidget, public Object {
		Q_OBJECT

	public:
		Camera(Device*, Grids::Value*, QWidget* parent = 0);

		void doMovement(QKeyEvent*, QMouseEvent*, QWheelEvent*);
		
		void setTarget( Vec3D );
		Vec3D getTarget();
		
    /* Required functions from Kal::Object */
    bool detectSelection();
    void select();
    void deselect();
    void draw(Device*);

	public slots:
		// The primary camera, uses the first 3 slots exclusively
		void mouseMoved(QMouseEvent *event);
		void keyboardPressed(QKeyEvent * event);
    void keyboardReleased(QKeyEvent * event);
    /* This slot is used for cameras that display other people on the network's view */
		void updatePosition(Vec3D, Vec3D, Vec3D);

	protected:
		void paintEvent(QPaintEvent*);		
		void keyPressEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
		void wheelEvent(QWheelEvent*);
		
	private:
    void parseAttrFromValue(const Grids::Value&);
    void parseAttr(const Grids::Value&);

		Device* d;

		bool mouse_pressed;
		Vec3D target;

		QMutex target_mutex;
	};

} // end namespace Kaleidoscope
