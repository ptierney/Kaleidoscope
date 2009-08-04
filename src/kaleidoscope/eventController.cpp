
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/spaceText.h>
#include <grids/interface.h>
#include <grids/objectController.h>
#include <iostream>

namespace Kaleidoscope {

	EventController::EventController( Device* d ) {
		this->d = d;
		
		mouse_button_mutex = SDL_CreateMutex();
		mouse_down_mutex = SDL_CreateMutex();
	}

	// This function should (and can) only be called
	// from the main thread.	
	void EventController::checkEvents( ) {
		mouse_button = 0;

		while( SDL_PollEvent( &event ) ) {
			if( event.type == SDL_KEYDOWN ) {
				parseKeyDown( event.key.keysym.sym );
			} 
			else if( event.type == SDL_MOUSEBUTTONDOWN ) {
				// record the first state
				int x, y;
				SDL_GetMouseState( &x, &y );

				down_x = x;
				down_y = y;

				mouse_button = event.button.button;
				mouse_down = true;
			} 
			else if( event.type == SDL_MOUSEBUTTONUP ) {
				// if the first click is the same as up then
				int x, y;
				SDL_GetMouseState( &x, &y );

				if(down_x == x && down_y == y &&
				   event.button.button != SDL_BUTTON_WHEELUP && event.button.button != SDL_BUTTON_WHEELDOWN ){
					//d->getGui()->registerClick( SDL_BUTTON_LEFT, x, y );

					d->getObjectController()->detectSelectionThreaded( d );
				}
				mouse_down = false;
			}
			else if (event.type == SDL_QUIT ) {
				d->setRunning( 0 );
			} // end SDL_QUIT
		} // end while SDL_PollEvent 
	} // end checkEvents function

	void EventController::parseKeyDown( int key ){
		if( key == SDLK_TAB ) {
			d->getOSWindow()->getCamera()->swapCameraType();
		} else if( key == SDLK_ESCAPE ) {
			d->setRunning( 0 );
		} else if( key == SDLK_1 ){
			d->getInterface()->requestCreateRoom();
		}else if( ( key >= SDLK_a && key <= SDLK_z ) || key == SDLK_SPACE ) {
			std::cout << (char)key << std::endl;
		} else if( key == SDLK_2 ){
			SpaceText::requestCreate( d, "Hello World", 100u, Vector3( 50.f, 0.f, 50.f ) );
		}
	}
	
	Uint8 EventController::getMouseButton(){
		Uint8 temp_int;
		
		SDL_LockMutex( mouse_button_mutex );
		temp_int = mouse_button;
		SDL_UnlockMutex( mouse_button_mutex );
		
		return temp_int;
	}
	
	bool EventController::getMouseDown(){
		bool temp_bool;
		
		SDL_LockMutex( mouse_down_mutex );
		temp_bool = mouse_down;
		SDL_UnlockMutex( mouse_down_mutex );

		return temp_bool;
	}

} // end namespace Kaleidoscope
