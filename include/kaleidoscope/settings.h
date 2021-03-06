
#pragma once


namespace Kaleidoscope {
        class Device;

	// This ideally could be serialized / written to disk,
	// so that settings can be read back at a later time.
	class Settings {
	public:
		Settings();
		
		float getRoomWidth();
		
		friend class Device;
	private:
		float room_width;

	}; 

} // end namespace Kaleidoscope
