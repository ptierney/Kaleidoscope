

#include <grids/objectController.h>
#include <grids/define.h>

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

} // end namespace Grids
