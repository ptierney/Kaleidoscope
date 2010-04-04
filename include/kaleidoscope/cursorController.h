
#pragma once

#include <kaleidoscope/define.h>
#include <QWidget>
#include <QMouseEvent>

namespace Kaleidoscope {
	class Device;

	class CursorController {
	public:
		CursorController(Device*, QWidget*);

		// 0.5f, 0.5f sets the cursor the the center of the window
		void setPosition( float, float );
		void setToCenter();
		
		Vec2D getRelativePosition();
    Vec2D getRelativePosition(QMouseEvent*);
		int getMouseX();
		int getMouseY();
    void hideCursor();
    void showCursor();

	private:	
		int mouse_x;
		int mouse_y;
		
		Device* d;
		QWidget* widget;
	};

} // end namespace Kaleidoscope

