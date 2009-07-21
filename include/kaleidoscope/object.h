
#pragma once

#include <grids/object.h> 


namespace Kaleidoscope {
	class Device;

	class Object : public Grids::Object {

	public:
		Object( Device*, Grids::Value* );

	};

} // end namespace Kaleidoscope
