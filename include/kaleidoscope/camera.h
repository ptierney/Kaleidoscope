
#pragma once

#include <kaleidoscope/define.h>
#include <grids/define.h>
#include <kaleidoscope/object.h>
#include <QGLWidget>
#include <QTimer>

namespace Kaleidoscope {
        class CursorController;

        class Camera : public QGLWidget, public Object {
		Q_OBJECT

	public:
		Camera(Device*, Grids::Value*, QWidget* parent = 0);
                ~Camera();
                QSize sizeHint() const;

		void doMovement(QKeyEvent*, QMouseEvent*, QWheelEvent*);
		
		// Changes the camera from FPS to Maya and vice versa
		void swapCameraType();

                void setPerspective(float, float, float, float );
                void callGluLookAt();
		void setTarget( Vec3D );
		void setUp( Vec3D );
		void setType( int );
		void setCenterOfRotation( Vec3D );
		Vec3D getTarget();
		Vec3D getUp();
		Vec3D getCenterOfRotation();
		int getType();
		
                void draw(Device*);
		
                void resizeScene(unsigned int new_width, unsigned int new_height);
		void lookAtPoint( Vec3D );

                void initializeGL();
                void resizeGL(int, int);

	public slots:
		// The primary camera, uses the first 3 slots exclusively
		void mouseMoved(QMouseEvent *event);
		void keyboardPressed(QKeyEvent * event);
                void keyboardReleased(QKeyEvent * event);
		// This slot is used for cameras that display other people on the network's view
		void updatePosition(Vec3D, Vec3D, Vec3D);

	protected:
		void doMovementFPS(QKeyEvent*, QMouseEvent*, QWheelEvent*);
		void doMovementMaya(QKeyEvent*, QMouseEvent*, QWheelEvent*);

		int getType( Device* );
		void setType( Device* );
		void setLook( Vec3D);
		// These functions should be used whenever the user is accessing of of the 4 perspective functions:
		// fov, aspect, z_near, z_far
		void lockPerspective();
		void unlockPerspective();

		void paintEvent(QPaintEvent*);		
		void keyPressEvent(QKeyEvent*);
                void mousePressEvent(QMouseEvent*);
                void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
		void wheelEvent(QWheelEvent*);
		
	private:
		void parseAttrFromValue( Grids::Value* );
		void parseAttr( Grids::Value* );
		Vec3D rotateAroundAxis( Vec3D in_vec, Vec3D axis, float theta );
		Vec3D findRotationFromVector( Vec3D );

		Device* d;
		CursorController* controller;
		int type;
		int zoom_type;
		float rotate_speed;
		float translate_speed;
		float move_speed;
		float zoom_speed;
		float max_vertical_angle;
		float fov;
		float aspect;
		float z_near;
		float z_far;				
		int last_animation_time;
		bool mouse_pressed;
                int mouse_button;

		bool translating;
		bool rotating;
		bool zooming;
		
		Vec3D target;
		Vec3D up;
		// TO DO: center_of_rotation is not thread safe
		Vec3D center_of_rotation;
		Vec3D target_normal;
		Vec2D cursor_save;

                QTimer animation_timer;

                QMutex angle_mutex;
                QMutex speed_mutex;
		QMutex target_mutex;
		QMutex up_mutex;
		QMutex type_mutex;
		QMutex center_mutex;
		QMutex perspective_mutex;
	};

} // end namespace Kaleidoscope
