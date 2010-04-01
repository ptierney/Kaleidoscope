
#pragma once

#include <QKeyEvent>

/* So this would be a nice interface for collecing
   focus code, but it needs to have focus to receive events.
   */

namespace Kaleidoscope {
	class Device;

  class EventController {
	public:
    EventController(Device*);

    void keyPressEvent(QKeyEvent*);

	protected:

		
	private:
    Device* d_;
	};

} // end namespace Kaleidoscope
