
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/spaceText.h>
#include <grids/interface.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>
#include <iostream>
#include <QKeyEvent>

namespace Kaleidoscope {

	EventController::EventController(Device* d, QWidget* parent)
		: QWidget(parent) {
		this->d = d;		
	}

	void EventController::keyPressEvent(QKeyEvent* event){
		if(event->key() == Qt::Key_Escape)
			d->app->quit();
		else if(event->key() == Qt::Key_1)
			d->getInterface()->requestCreateRoom();
	}

} // end namespace Kaleidoscope
