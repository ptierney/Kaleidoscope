
#pragma once

#include <kaleidoscope/glSpaceObject.h>

namespace Kaleidoscope {
	class Device;

	class Room : public GLSpaceObject {

	public:
		Room( Device*, Grids::Value* );

		void draw( Device* );
		void create( Device*, Grids::Value* );

		static void requestCreateRoom( Device*, float room_size );
			
		void placeRoom( Device* );
		void buildRoom( Device* );
	};

} // end namespace Kaleidoscope

