
#include <iostream>

#include <QKeyEvent>
#include <QString>

#include <kaleidoscope/eventController.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/inputTete.h>
#include <kaleidoscope/inputTextNode.h>
#include <grids/utility.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  EventController::EventController(Device* d) {
    d_ = d;
    recording_keys_ = false;
	}

  // EventController isn't derived from a QObject, this function
  // is called by Scene2D, not the Qt stack.
  void EventController::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Escape){
      d_->app->quit();
      return;
    }

    key_queue_ += event->text().toStdString();

    if(recording_keys_)
      return;

    // Is the user framed on a node?
    // if d_->chat_controller()->reframing()
    if(d_->chat_controller()->last_selected()){
      key_queue_.clear();
      key_queue_ += event->text().toStdString();

      Tete* framed_node = d_->chat_controller()->last_selected();
      GridsID node_id = framed_node->id();
      GridsID chat_id = framed_node->chat_id();

      recording_keys_ = true;

      GridsID new_node_id = Tete::requestCreate(d_, node_id, chat_id,
                                                "", Vec3D() );
      Link::requestCreate(d_, new_node_id, node_id);

    } // We are not framed on a node and not in a chat
    else {
      // rock my world
      key_queue_.clear();
      key_queue_ += event->text().toStdString();

      GridsID chat_id = d_->chat_controller()->default_chat_id();
      GridsID parent_id;

      Chat* chat = d_->chat_controller()->getChatFromID(chat_id);
      if(chat != NULL){
        if(!chat->tetes().empty()){
          parent_id = chat->tetes().back()->id();
        }
      }

      recording_keys_ = true;

      GridsID new_node_id = Tete::requestCreate(d_, parent_id, chat_id,
                                                "", Vec3D() );
    }

	}

  void EventController::start_key_queue(){
    key_queue_.clear();
  }

  void EventController::reset_key_queue(){
    key_queue_.clear();
  }

  std::string EventController::stopRecordingKeys(){
    std::string temp = key_queue_;
    key_queue_.clear();
    recording_keys_ = false;
    return temp;
  }

  std::string EventController::key_queue(){
    return key_queue_;
  }

} // end namespace Kaleidoscope
