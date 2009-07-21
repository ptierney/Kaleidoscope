

#pragma once

#include <kaleidoscope/define.h>

#include <vector>

namespace Grids{
	class ObjectController;
	class Interface;
	class Utility;
}

namespace Kaleidoscope {
	class Room;

	class Device {
	public:
		Device(); // Default to 640x480
		Device( unsigned int screen_width, unsigned int screen_height );
		~Device();
		
		Grids::ObjectController* getObjectController();
		Grids::Interface* getInterface();
		Grids::Utility* getGridsUtility();

	private:
		Grids::ObjectController* object_controller;
		Grids::Interface* interface;
		Grids::Utility* g_utility;

		void init( unsigned int, unsigned int );
	};



} // end namespace Kaleidoscope
