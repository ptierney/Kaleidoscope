
#include <grids/objectController.h>
#include <grids/define.h>
#include <grids/gObject.h>
#include <grids/event.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/spaceText.h>
#include <kaleidoscope/inputTextItem.h>
#include <QMutex>
#include <QMutexLocker>
#include <QMouseEvent>

namespace Grids {
	
	ObjectController::ObjectController(Kal::Device* d_, QWidget* parent)
		: QWidget(parent) {
		d = d_;
	}
	
	void ObjectController::registerObject( GridsID new_id, Object* new_ptr ){
                QMutexLocker lock(&map_mutex);
		object_ids.push_back( new_id );
		id_ptr_map[ new_id ] = new_ptr;
		ptr_id_map[ new_ptr ] = new_id;
	}

	// Returns an object's ID given it's pointer
	GridsID ObjectController::getIDFromPointer( Object* obj_ptr ){
                QMutexLocker lock(&map_mutex);
		return ptr_id_map[ obj_ptr ];
	}
	
	// Returns an object's pointer given it's ID
	Object* ObjectController::getPointerFromID( GridsID obj_id ){
                QMutexLocker lock(&map_mutex);
		return id_ptr_map[ obj_id ];
	}

	void ObjectController::createObject( GridsID new_id, Event* evt ){
		if( !knownObject( new_id, evt ) )
			createGenericObject( new_id, evt );
	}
	
	// Thoughs: this function should be replaced by a hash.  Each object
	// "registers" both it's name and ... is this possible?
	bool ObjectController::knownObject( GridsID new_id, Event* evt ) {
		std::string type = (*(evt->getArgsPtr()))[ "req" ][ "attr" ][ "type" ].asString();
		bool found = 0;
		
		if( type == "Camera" ){
			d->registerCamera(evt->getArgsPtr());
			found = 1;
                    } else if( type == "InputText") {
                        Kal::InputTextItem::gridsCreate(d, evt);
                        found = 1;
                    }

                /*else if( type == "SpaceText" ) {
		    new Kal::SpaceText( d, evt->getArgsPtr() );
		    found = 1;
		    }*/

		return found;
	}
	
	void ObjectController::createGenericObject( GridsID new_id, Event* evt ){
		// Create a cube with a questionmark or something
	}
	
	void ObjectController::registerCamera( GridsID new_id, Event* evt ){
            // Each camera is created inside of a new window, therefore the device must
            // create the camera.
	}

	void ObjectController::updateObjectPosition( GridsID in_id, Vec3D pos ){
		getPointerFromID( in_id )->setLocalPosition(pos);
	}

	void ObjectController::updateObjectRotation( GridsID in_id, Vec3D rot ){
		getPointerFromID( in_id )->setLocalRotation( rot );
	}

	void ObjectController::updateObjectScale( GridsID in_id, Vec3D scl ){
		getPointerFromID( in_id )->setLocalScale( scl );
	}

	void ObjectController::updateObjectAttr( GridsID in_id, Event* evt ){
                getPointerFromID( in_id )->updateAttr(evt);
	}

	void ObjectController::mouseReleasedEvent(QMouseEvent* event) {
		detectSelectionThreaded(event->x(), event->y());
	}
	
	void ObjectController::detectSelectionThreaded(int mouse_x, int mouse_y){

	}

} // end namespace Grids
