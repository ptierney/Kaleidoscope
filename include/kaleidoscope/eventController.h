
#pragma once

#include <QWidget>

namespace Kaleidoscope {
	class Device;

	class EventController : public QWidget {
		Q_OBJECT

	public:
		EventController(Device*, QWidget* parent = 0);

	protected:
		keyPressEvent(QKeyEvent*);
		
	private:
		Device* d;
	};

} // end namespace Kaleidoscope
