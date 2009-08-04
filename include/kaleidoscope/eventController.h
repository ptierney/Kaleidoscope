
#pragma once

#include <SDL/SDL.h>

namespace Kaleidoscope {
	class Device;

	class EventController {
	public:
		EventController(Device*);
		
		void checkEvents( );

		Uint8 getMouseButton();
		bool getMouseDown();

	private:
		void parseKeyDown( int );

		Device* d;
		SDL_Event event;

		int down_x;
		int down_y;
		int mouse_button;
		bool mouse_down;
	
		SDL_mutex* mouse_button_mutex;
		SDL_mutex* mouse_down_mutex;
	};

} // end namespace Kaleidoscope
