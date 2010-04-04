
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

    std::string key_queue();
    void reset_key_queue();
    void start_key_queue();
    std::string stopRecordingKeys();

	protected:

		
	private:
    Device* d_;
    bool recording_keys_;
    std::string key_queue_;
	};

} // end namespace Kaleidoscope
