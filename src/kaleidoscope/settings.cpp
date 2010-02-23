
#include <kaleidoscope/settings.h>


namespace Kaleidoscope {
	
	Settings::Settings(){
		room_width = 100.0f;
	}

	float Settings::getRoomWidth(){ return room_width; }

} // end namespace Kaleidoscope
