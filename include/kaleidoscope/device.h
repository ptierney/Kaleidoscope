

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
		
		Grids::ObjectController* getObjectController();
		Grids::Interface* getInterface();
		Grids::Utility* getGridsUtility();
		OSWindow* getOSWindow();		
		EventController* getEventController();
		Settings* getSettings();

		bool getRunning();
		void setRunning( bool );
		GridsID getMyID();
		GridsID getMyRoom();
		void setMyID(GridsID);
		void setMyRoom(GridsID);
		
	private:
		Grids::ObjectController* object_controller;
		Grids::Interface* interface;
		Grids::Utility* g_utility;
		OSWindow* window;
		EventController* event_controller;
		Settings* settings;

		void createObjects( unsigned int, unsigned int );
		void init( unsigned int, unsigned int );
		void loadRoom();
		
		GridsID my_id;		
		GridsID my_room;

		bool running;
		SDL_mutex* running_mutex;
		SDL_mutex* my_id_mutex;
		SDL_mutex* my_room_mutex;
	};


} // end namespace Kaleidoscope
