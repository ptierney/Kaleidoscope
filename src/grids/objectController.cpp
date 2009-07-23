

#include <grids/objectController.h>
#include <grids/define.h>
#include <grids/object.h>
#include <grids/event.h>

namespace Grids {
	
	ObjectController::ObjectController(){

	}
	
	void ObjectController::registerObject( GridsID new_id, Object* new_ptr ){
		lock();
		object_ids.push_back( new_id );
		id_ptr_map[ new_id ] = new_ptr;
		ptr_id_map[ new_ptr ] = new_id;
		unlock();
	}

	// Returns an object's ID given it's pointer
	GridsID ObjectController::getIDFromPointer( Object* obj_ptr ){
		return ptr_id_map[ obj_ptr ];
	}
	
	// Returns an object's pointer given it's ID
	Object* ObjectController::getPointerFromID( GridsID obj_id ){
		return id_ptr_map[ obj_id ];
	}

	void ObjectController::createObject( GridsID new_id, Event* evt ){
		if( !knownObject( new_id, evt ) )
			createGenericObject( new_id, evt );
	}
	
	bool ObjectController::knownObject( GridsID new_id, Event* evt ) {
		std::string type = (*(evt->getArgsPtr()))[ "req" ][ "attr" ][ "type" ].asString();
		bool found = 0;
		
		if( type == "Camera" ){
			d->getOSWindow()->registerCamera( new Kal::Camera
			found = 1;
		}

		return found;
	}
	
	void ObjectController::createGenericObject( GridsID new_id, Event* evt ){
		// Create a cube with a questionmark or something
	}

	void ObjectController::updateObjectPosition( GridsID in_id, Vec3D pos ){
		getPointerFromID( in_id )->setLocalPosition( pos );
	}

	void ObjectController::updateObjectRotation( GridsID in_id, Vec3D rot ){
		getPointerFromID( in_id )->setLocalRotation( rot );
	}

	void ObjectController::updateObjectScale( GridsID in_id, Vec3D scl ){
		getPointerFromID( in_id )->setLocalScale( scl );
	}

	void ObjectController::updateObjectAttr( GridsID in_id, Event* evt ){
		getPointerFromID( in_id )->setAttrFromValue( evt->getArgsPtr() );
	}

} // end namespace Grids
