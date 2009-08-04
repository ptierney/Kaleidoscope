
#pragma once

#include <kaleidoscope/define.h>

#include <grids/SDLObject.h>

namespace Kaleidoscope {
	class Device;

	class CursorController : public Grids::SDLObject {
	public:
		CursorController( Device* );

		// 0.5f, 0.5f sets the cursor the the center of the window
		void setPosition( float, float );
		void setToCenter();
		
		Vec2D getRelativePosition();
		int getMouseX();
		int getMouseY();

	private:	
		int mouse_x;
		int mouse_y;
		
		Device* d;
	};

} // end namespace Kaleidoscope
