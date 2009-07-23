

#pragma once

#include <kaleidoscope/define.h>
#include <SDL/SDL_thread.h>

#include <vector>

namespace Grids{
	class ObjectController;
	class Interface;
	class Utility;
}

namespace Kaleidoscope {
	class Room;
	class OSWindow;
	class EventController;
	class Camera;
	class Settings;

	class Device {
	public:
		Device(); // Default to 640x480
		Device( unsigned int screen_width, unsigned int screen_height );
		~Device();

		void run();
		bool getRunning();
		void setRunning( bool );
		
		Grids::ObjectController* getObjectController();
		Grids::Interface* getInterface();
		Grids::Utility* getGridsUtility();
		OSWindow* getOSWindow();		
		EventController* getEventController();
		Settings* getSettings();
		
		GridsID getMyID();
		void setMyID(GridsID);

	private:
		Grids::ObjectController* object_controller;
		Grids::Interface* interface;
		Grids::Utility* g_utility;
		OSWindow* window;
		EventController* event_controller;
		Settings* settings;

		void createObjects( unsigned int, unsigned int );
		void init( unsigned int, unsigned int );
		
		GridsID my_id;		

		bool running;
		SDL_mutex* running_mutex;
		SDL_mutex* my_id_mutex;
	};


} // end namespace Kaleidoscope
