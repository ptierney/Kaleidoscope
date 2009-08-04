
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/device.h>

#include <grids/SDLObject.h>

#include <SDL/SDL.h>

namespace Kaleidoscope
{
	CursorController::CursorController( Device* dvc ) : Grids::SDLObject() {
		d = dvc;		
	}	

	void CursorController::setPosition( float xScale, float yScale ) {
		SDL_WarpMouse( (int)( xScale * d->getOSWindow()->getWidth() ), 
					(int)(  yScale * d->getOSWindow()->getHeight() ) );
	}
	
	void CursorController::setToCenter( ) {
		setPosition( 0.5f, 0.5f );
	}
	
	int CursorController::getMouseX(){
		int temp_x;
		lock();
		temp_x = mouse_x;
		unlock();
		
		return temp_x;
	}

	int CursorController::getMouseY(){
		int temp_y;
		lock();
		temp_y = mouse_y;
		unlock();
		
		return temp_y;
	}
	
	Vec2D CursorController::getRelativePosition( ) {
		int x, y;
		SDL_GetMouseState( &x, &y );
			
		lock();
		mouse_x = x;
		mouse_y = y;
		unlock();
							
		return Vec2D( (float)( x ) / (float) d->getOSWindow()->getWidth(), 
				    (float)( y ) / (float) d->getOSWindow()->getHeight() );	
	}


} // end namespace Kaleidoscope
