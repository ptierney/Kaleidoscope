

#pragma once

#include <kaleidoscope/define.h>
#include <grids/objectController.h>

namespace Kaleidoscope {

	class Device {
	public:
		Device(); // Default to 640x480
		Device( unsigned int screen_width, unsigned int screen_height );
		~Device();
		
		Grids::ObjectController* getObjectController();
		
	private:
		Grids::ObjectController* object_controller;

		void init( unsigned int, unsigned int );
	};



} // end namespace Kaleidoscope
