
#include <kaleidoscope/spaceObject.h>
#include <kaleidoscope/object.h>

namespace Kaleidoscope {

	SpaceObject::SpaceObject( Device* d, Grids::Value* val ) : Object( d, val ) {

	}

	bool SpaceObject::detectSelection(){
    return false;
	}

} // end namespace Kaleidoscope
