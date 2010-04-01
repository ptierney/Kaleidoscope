
#include <iostream>

#include <QKeyEvent>

#include <kaleidoscope/eventController.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/inputTete.h>
#include <kaleidoscope/inputTextNode.h>
#include <grids/utility.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  EventController::EventController(Device* d) {
    d_ = d;
	}

  void EventController::keyPressEvent(QKeyEvent* event){
		if(event->key() == Qt::Key_Escape)
      d_->app->quit();
    else{
      d_->getNoticeWindow()->write(7, "Keypress");
      // Request a new InputTete, making a new chat, and
      // giving it a blank parent
      InputTete::requestCreate(d_,
                               d_->getGridsUtility()->getNewUUID(),
                               "");

    }
	}

} // end namespace Kaleidoscope
