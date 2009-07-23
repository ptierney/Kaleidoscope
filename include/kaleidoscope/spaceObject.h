
#pragma once

#include <kaleidoscope/object.h>

namespace Kaleidoscope {

	class SpaceObject : public Object {

	public:
		SpaceObject( Device*, Grids::Value* );
		
		virtual bool detectSelection();


	};

} // end namespace Kaleidoscope
