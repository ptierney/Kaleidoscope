
/* Current Bugs:
 *
 * 2. Radeon 22 error / segfault at startup
 * 
 *
 *
 */


#include <kaleidoscope/camera.h>
#include <grids/object.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/renderer.h>
#include <grids/irrMath.h>
#include <SDL/SDL_opengl.h>
#include <iostream>

namespace Kaleidoscope {

	Camera::Camera(Device* d, Grids::Value* val, QWidget* parent) 
		: Object( d, val ), QGLWidget(parent) {
		parseAttrFromValue(val);
		last_animation_time = d->getTicks();
	
		// Create a dockable window in the main window, and place this 
		// camera inside of it.  Make in the main camera if there are
		// no other cameras already.
		QDockWidget *dock = new QDockWidget(tr("Camera"), d->main_window);		
		if(d->main_camera == 0)
			d->main_camera = this;

		dock->setWidget(this);
		d->main_window->addDockWidget(Qt::LeftDockWidgetArea, dock);
	}
	
	void Camera::setPerspective( Device* d, float fov, float aspect, float z_near, float z_far ) {
		QMutexLocker persp_lock(&perspective_mutex);
		this->fov = fov;	
		this->aspect = aspect;
		this->z_near = z_near;
		this->z_far = z_far;

		QMutexLocker gl_lock(d->getOSWindow()->getRenderer()->getGLMutex());
		gluPerspective( fov, aspect, z_near, z_far );
	}	

	void Camera::callGluLookAt( Device* d ) {
		Vec3D temp_target = getTarget();
		Vec3D temp_pos = getPosition();
		Vec3D temp_up = getUp();

		d->getOSWindow()->getRenderer()->lockGL();
		gluLookAt( temp_target.X, temp_target.Y, temp_target.Z,
				 temp_pos.X, temp_pos.Y, temp_pos.Z,
				 temp_up.X, temp_up.Y, temp_up.Z );
		d->getOSWindow()->getRenderer()->unlockGL();

	}
	
	void Camera::parseAttrFromValue( Grids::Value* val ){
		parseAttr( getAttrFromValue( val ) );
	}
	
	void Camera::parseAttr( Grids::Value* val ){
		lock();

		Vec3D temp_target = Vec3D( (*val)[ "target" ][ 0u ].asDouble(), 
							  (*val)[ "target" ][ 1u ].asDouble(),
							  (*val)[ "target" ][ 2u ].asDouble() ).normalize() + getPosition();
		temp_target.normalize();
		temp_target *= -1.0f;		
		setTarget( temp_target );
		
		setUp( Vec3D( (*val)[ "up" ][ 0u ].asDouble(), 
				    (*val)[ "up" ][ 1u ].asDouble(),
				    (*val)[ "up" ][ 2u ].asDouble() ) );
		
		type = (*val)[ "camera_type" ].asInt();
		if( type == FPS )
			SDL_ShowCursor( 0 );
		else if( type == MAYA )
			SDL_ShowCursor( 1 );

		rotate_speed = (*val)[ "rotate_speed" ].asDouble();
		translate_speed = (*val)[ "translate_speed" ].asDouble();		
		move_speed = 1.0f; 
		max_vertical_angle = 89.0f;
		zoom_speed = 0.06f;
		center_of_rotation = Vec3D( 0.0f, 0.0f, 0.0f );
		zoom_type = ZOOM_CENTER;

		translating = false;
		rotating = false;

		unlock();
	}

	void Camera::swapCameraType( ){
		if( getType( ) == FPS ){
			setType( MAYA );
			Vec3D new_center =  getTarget() - getLocalPosition() ;
			new_center.normalize();
			new_center *= -35.0f ;
			//setCenterOfRotation( getLocalPosition() + new_center );
			setCenterOfRotation( Vec3D( 0.f, 0.f, 0.f ) );
			/*Vec3D temp_target = getPosition() - getCenterOfRotation();
			temp_target.normalize();
			temp_target += getPosition();
			setTarget( temp_target );*/ 
			lookAtPoint( getCenterOfRotation() );
		}
		else if ( getType() == MAYA ) {									
			setType( FPS );
			setUp( Vec3D( 0.0f, 1.0f, 0.0f ) );
			//Vec3D temp_target = getPosition() - getCenterOfRotation();
			//temp_target.normalize();
			//temp_target += getPosition();
			//setTarget( temp_target ); 
			lookAtPoint( getCenterOfRotation() );
			SDL_ShowCursor( 0 );
		}
	}
	 
	int Camera::getType() {
		int temp_type;
		
		SDL_LockMutex( type_mutex );
		temp_type = type;
		SDL_UnlockMutex( type_mutex );
		
		return temp_type;
	}
	
	void Camera::setType( int in_type ) {
		SDL_LockMutex( type_mutex );
		type = in_type;
		SDL_UnlockMutex( type_mutex );
	}
	
	void Camera::setCenterOfRotation( Vec3D vec ){
		SDL_LockMutex( center_mutex);
		center_of_rotation = vec;
		SDL_UnlockMutex( center_mutex );	
	}
	
	Vec3D Camera::getCenterOfRotation( ) {
		Vec3D vec;
		SDL_LockMutex( center_mutex );
		vec = center_of_rotation;
		SDL_UnlockMutex( center_mutex );

		return vec;
	}

	void Camera::doMovement( Device* d ){
		if( getType() == FPS )
			doMovementFPS( d );
		else if( getType() == MAYA )
			doMovementMaya( d );
	}

	void Camera::doMovementFPS( Device* d ){
		// get the current time
		int now = SDL_GetTicks(); 
		int time_diff =  now - last_animation_time;
		last_animation_time = now;

		// if framerate > 1000 fps errors could occur		
		if( time_diff == 0 )
			time_diff = 1;
			
		Vec3D temp_rotation = getRotation();
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		setTarget( Vec3D( 0.0f, 0.0f, 1.0f) );
		
		Vec2D cursorpos = d->getOSWindow()->getCursorController()->getRelativePosition();
		
		if( cursorpos != Vec2D( 0.5f, 0.5f ) ) {
			temp_rotation.Y += ( 0.5f - cursorpos.X) * rotate_speed * time_diff;
			temp_rotation.X -= ( 0.5f - cursorpos.Y) * rotate_speed * time_diff;
						
			d->getOSWindow()->getCursorController()->setPosition( 0.5f, 0.5f );
			
			if( temp_rotation.X > max_vertical_angle) temp_rotation.X = max_vertical_angle;
			if( temp_rotation.X < -max_vertical_angle) temp_rotation.X = -max_vertical_angle;
		}
			
		Matrix4 mat = Matrix4();
		
		mat.setRotationDegrees( Vec3D( -temp_rotation.X, temp_rotation.Y, 0.0f ) );
		
		Vec3D temp_irr_target = getTarget();
		mat.transformVect( temp_irr_target );
		
		temp_irr_target.normalize();
		
		setTarget( temp_irr_target );
		
		Uint8* keys;		
		keys = SDL_GetKeyState( NULL );
		
		if( keys[SDLK_UP] ) {
			setLocalPosition( getLocalPosition() - getTarget() * ( time_diff * move_speed ) );				
		}
		else if( keys[SDLK_DOWN] ){
			setLocalPosition( getLocalPosition() + getTarget() * ( time_diff * move_speed ) );
		}
			
		Vec3D strafevect = getTarget();
		strafevect = strafevect.crossProduct( getUp() );
		strafevect.normalize();
			
		if( keys[SDLK_LEFT] ){
			setLocalPosition( getLocalPosition() + strafevect * ( time_diff * move_speed ) );
		}
		else if(  keys[SDLK_RIGHT] ){
			setLocalPosition( getLocalPosition() - strafevect * ( time_diff * move_speed ) );	
		}
		
		setTarget( getTarget() + getLocalPosition() );		

		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		setLocalRotation( temp_rotation );		

		target_normal = ( getLocalPosition() - getTarget() ).normalize();
	}

	void Camera::doMovementMaya( Device* d ) {
		int now = SDL_GetTicks(); // get the current time
		int time_diff =  now - last_animation_time;
		last_animation_time = now;
				
		// The cached state is necessary for reading mouse wheel movements
		Uint8 mouse_state = SDL_GetMouseState(NULL, NULL);
		Uint8 cached_mouse_state = d->getEventController()->getMouseButton();
		//bool mouse_down = d->getEventController()->getMouseDown();

		if( time_diff == 0 )
			time_diff = 1;
		
		if( (cached_mouse_state & SDL_BUTTON( SDL_BUTTON_WHEELUP ) ) || (cached_mouse_state & SDL_BUTTON( SDL_BUTTON_WHEELDOWN ) ) ||
		    cached_mouse_state == SDL_BUTTON_WHEELUP || cached_mouse_state == SDL_BUTTON_WHEELDOWN ) {
			//std::cout << "wheel\n";
			Vec3D zoom_vector;
			Vec3D position_difference;
			Vec3D target_difference;
			
			if( zoom_type == ZOOM_FORWARD ) {
				Vec3D normal_target = getTarget();
				normal_target.normalize();
				
				if( cached_mouse_state == SDL_BUTTON_WHEELUP ) {
					zoom_vector = normal_target * ( time_diff * zoom_speed * -1.0f  );
				}
				else {
					zoom_vector = normal_target * ( time_diff * zoom_speed );
				}

				setLocalPosition( getLocalPosition() + zoom_vector );
				setTarget( getTarget() + zoom_vector );				
			}
			else if( zoom_type == ZOOM_CENTER ) {
				position_difference = getLocalPosition() - getCenterOfRotation();
				target_difference = getTarget() - getCenterOfRotation();
				
				float scale_amount;
				
				// Zoom Closer				
				if( cached_mouse_state == SDL_BUTTON_WHEELUP ) {
					scale_amount = 1.0f + time_diff * zoom_speed * -1.0f ;
				}
				// Zoom Out
				else if( cached_mouse_state == SDL_BUTTON_WHEELDOWN ) {
					scale_amount = 1.0f + time_diff * zoom_speed;
				}
				
				position_difference *= scale_amount;
				target_difference *= scale_amount;
				
				setLocalPosition( getCenterOfRotation() + position_difference );
				setTarget( getCenterOfRotation() + target_difference );
				
				Vec3D temp_tar =  getTarget() - getLocalPosition(); // The target must be normalized for selection / cursor-ray finding to work
				temp_tar.normalize();
				setTarget( getLocalPosition() + temp_tar );
				
				//lookAtPoint( getCenterOfRotation() );
			}
		}
		
		// SDL poll event		
		//NOTE: The mouse cursor should be hidden when the mouse is dragged
		Vec2D cursor_pos = d->getOSWindow()->getCursorController()->getRelativePosition();
		
		if( cursor_pos != Vec2D( 0.5f, 0.5f ) ) {
			//  TRANSLATION
			if( mouse_state & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) {				
				if( !translating ) {
					// This allows the mouse to hover around and click on items.  Then, when the 
					// mouse is pressed, the view changes.
					cursor_save = cursor_pos;
					d->getOSWindow()->getCursorController()->setToCenter();
					
					translating = true;
					SDL_ShowCursor( 0 );
				}
				else {
					//std::cout << "Difference "<< SDL_GetTicks() << std::endl;
					float offset_x = ( cursor_pos.X - 0.5f ) * time_diff;
					float offset_y = ( cursor_pos.Y - 0.5f ) * time_diff;
						
						
					Vec3D strafe_vector = getTarget().crossProduct( getUp() );
					Vec3D elevation_vector = getTarget().crossProduct( strafe_vector );
					strafe_vector.normalize();
					elevation_vector.normalize();
						
					// Implamented so that you pan faster when you are farther away from the room, etc
					float dist_to_center = getLocalPosition().getDistanceFrom( getCenterOfRotation() );
						
					Vec3D translateHorizontle = strafe_vector * (   translate_speed * offset_x  * 1000.0f * dist_to_center / 50.0f );
					Vec3D translateVertical = elevation_vector * ( -1.0f * translate_speed * offset_y * 1000.0f * dist_to_center / 50.0f );
						
					Vec3D translate_amount = translateHorizontle + translateVertical;
						
					setLocalPosition( getLocalPosition() + translate_amount );
					setTarget( getTarget() + translate_amount );
					//setUp( getUp() + translate_amount );
					setCenterOfRotation( getCenterOfRotation() + translate_amount );
						
					d->getOSWindow()->getCursorController()->setToCenter();
				}
			}
			// ROTATION 
			else if( mouse_state & SDL_BUTTON( SDL_BUTTON_LEFT ) ) {
				if( !rotating ){
					cursor_save = cursor_pos;
					d->getOSWindow()->getCursorController()->setToCenter();
										
					rotating = true;
					
					SDL_ShowCursor( 0 );
				}
				else {
					float offset_x = (cursor_pos.Y - 0.5f) * time_diff * rotate_speed * -0.1f ;
					float offset_y = (cursor_pos.X - 0.5f) * time_diff * rotate_speed * -0.1f ;

					Vec3D strafe_vector = getTarget().crossProduct( getUp() );
					Vec3D elevation_vector = getTarget().crossProduct( strafe_vector );
					strafe_vector.normalize();
					elevation_vector.normalize();
						
					setLocalPosition( getLocalPosition() - getCenterOfRotation() );
					setTarget( getTarget() - getCenterOfRotation() );
						
					setLocalPosition( rotateAroundAxis( getLocalPosition(), elevation_vector, -1.0f*offset_y ) );
					setLocalPosition( rotateAroundAxis( getLocalPosition(), strafe_vector, -1.0f*offset_x ) );
						
					setTarget( rotateAroundAxis( getTarget(), elevation_vector, -1.0f*offset_y ) );
					setTarget( rotateAroundAxis( getTarget(), strafe_vector, -1.0f*offset_x ) );
						
					setLocalPosition( getLocalPosition() + getCenterOfRotation() );
					setTarget( getTarget() + getCenterOfRotation() );

					d->getOSWindow()->getCursorController()->setToCenter();
				} // end d->Rotating == true
			} 
		}
		else {
			/*
			if( !( (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) ) ||
				  (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT) ) ) ) {	
			*/
			if( !( d->getEventController()->getMouseDown() ) ) {
				if( translating || rotating || zooming ) {
					d->getOSWindow()->getCursorController()->setPosition( cursor_save.X, cursor_save.Y );
				}
								
				translating = false;
				rotating = false;
				zooming = false;
								
				SDL_ShowCursor( 1 );
			}
		} 
	}

	void Camera::draw( Device* d ){

	}

     void Camera::resizeScene( Device * d, unsigned int new_width, unsigned int new_height )
     {
		 d->getOSWindow()->getRenderer()->lockGL();
          glViewport(0, 0, new_width, new_height);

          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
		 
		 d->getOSWindow()->getRenderer()->unlockGL();

          // NOTE: It is possible to calulate the exact (most realistic) fovy of a progmam
          // based on the viewer's distance from the screen.  See OpenGL documentation.
          // 26.0f *roughly* coresponds to a 35mm camera 50mm lens,  45.0f *roughly* coresponds to a 24mm wide angle lense.
          float fovy = 26.0f;

          float aspecty = (GLfloat)new_width/(GLfloat)new_height;
          float zNear = 0.1f;
          float zFar = 10000.0f;

          setPerspective( d, fovy, aspecty, zNear, zFar );

		 d->getOSWindow()->getRenderer()->lockGL();
          glMatrixMode(GL_MODELVIEW);
		 d->getOSWindow()->getRenderer()->unlockGL();

          //d->getOSWindow()->setWidth( new_width );
          //d->getOSWindow()->setHeight( new_height );
     }
	
	Vec3D Camera::rotateAroundAxis( Vec3D in_vector, Vec3D axis, float theta ) {
		float ux = axis.X * in_vector.X;
		float uy = axis.X * in_vector.Y;
		float uz = axis.X * in_vector.Z;
		float vx = axis.Y * in_vector.X;
		float vy = axis.Y * in_vector.Y;
		float vz = axis.Y * in_vector.Z;
		float wx = axis.Z * in_vector.X;
		float wy = axis.Z * in_vector.Y;
		float wz = axis.Z * in_vector.Z;
		
		double si = sin(theta);
		double co = cos(theta);
		
		float xx = (float) (axis.X
				* (ux + vy + wz)
				+ (in_vector.X * (axis.Y * axis.Y + axis.Z * axis.Z) - axis.X * (vy + wz))
				* co + (-wy + vz) * si);
		float yy = (float) (axis.Y
				* (ux + vy + wz)
				+ (in_vector.Y * (axis.X * axis.X + axis.Z * axis.Z) - axis.Y * (ux + wz))
				* co + (wx - uz) * si);
		float zz = (float) (axis.Z
				* (ux + vy + wz)
				+ (in_vector.Z * (axis.X * axis.X + axis.Y * axis.Y) - axis.Z * (ux + vy))
				* co + (-vx + uy) * si);
		return Vec3D(xx, yy, zz);  
	}

	// I think this should be the global position, I'll have to
	// test it to find out.
	void Camera::lookAtPoint( Vec3D vec ){
          setLook(vec - getLocalPosition() );
     }

     void Camera::setLook( Vec3D vec ){
          if( getType( ) == MAYA ){
			vec.invert();
			vec.normalize();	
			
			setTarget( getPosition() + vec ) ;
		}
          else if( getType( ) == FPS ){
			setLocalRotation( findRotationFromVector( vec ) );
		}
     }

     Vec3D Camera::findRotationFromVector( Vec3D VecIn ) {
          Vec3D angle = Vec3D();
          Vec3D VecIn2 = Vec3D( VecIn );

          VecIn2.invert();

          angle.Y = atan2( VecIn2.X, VecIn2.Z );
          angle.Y *= GRAD_PI;

          angle.Y *= -1.0f;

          float z1;
          z1 = sqrt( VecIn2.X * VecIn2.X + VecIn2.Z * VecIn2.Z );

          angle.X = atan2( z1, VecIn2.Y );
          angle.X *= GRAD_PI;
          angle.X -= 90.0f;

          return angle;
     }
	
	void Camera::lockPerspective(){
		SDL_LockMutex( perspective_mutex );
	}
	
	void Camera::unlockPerspective(){
		SDL_UnlockMutex( perspective_mutex );
	}








	//////////////////////////////
	// Accessor functions
		
	void Camera::setTarget( Vec3D vec ){ SDL_LockMutex( target_mutex ); target = vec; SDL_UnlockMutex( target_mutex ); }
	void Camera::setUp( Vec3D vec ){ SDL_LockMutex( up_mutex ); up = vec; SDL_UnlockMutex( up_mutex ); }
	
	Vec3D Camera::getTarget(){
		Vec3D temp_vec;
		
		SDL_LockMutex( target_mutex );
		temp_vec = target;
		SDL_UnlockMutex( target_mutex );
		return temp_vec;
	}
	    
	Vec3D Camera::getUp() {
		Vec3D temp_vec;
		
		SDL_LockMutex( up_mutex );
		temp_vec = up;
		SDL_UnlockMutex( up_mutex );
		
		return temp_vec;
	}


} // end namespace Kaleidoscope
