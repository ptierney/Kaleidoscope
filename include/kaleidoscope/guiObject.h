
#pragma once

#include <kaleidoscope/object.h>
#include <grids/define.h>

namespace Kaleidoscope {
	
	class GUIObject : public Object {

	public:
		GUIObject( Device*, Grids::Value* );

		virtual bool detectSelection();

	private:


	}; 

} // end namespace Kaleidoscope
