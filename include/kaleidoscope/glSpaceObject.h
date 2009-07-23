
#pragma once

#include <kaleidoscope/spaceObject.h>

namespace Kaleidoscope {

	class GLSpaceObject : public SpaceObject {

	public:
		GLSpaceObject( Device*, Grids::Value* );

		virtual void create( Device*, Grids::Value* );
		virtual void draw( Device* );

		void glDraw( Device* );
		void glDrawGeometry( Device* );
	};

} // end namespace Kaleidoscope
