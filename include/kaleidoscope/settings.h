
#pragma once

#include <kaleidoscope/device.h>

namespace Kaleidoscope {

	class Settings {
	public:
		Settings();
		
		float getRoomWidth();
		
		friend class Device;
	private:
		float room_width;

	}; 

} // end namespace Kaleidoscope
