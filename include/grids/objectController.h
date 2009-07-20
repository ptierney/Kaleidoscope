
#pragma once

#include <grids/object.h>
#include <grids/define.h>
#include <grids/SDLObject.h>

#include <map>
#include <vector>

namespace Grids {

	class ObjectController : public SDLObject {
	public:
		ObjectController();
		
		void registerObject( GridsID, Object* );
		GridsID getIDFromPointer( Object* );
		Object* getPointerFromID( GridsID );

	private:		
		std::vector< GridsID > object_ids;
		std::map< GridsID, Object* > id_ptr_map;
		std::map< Object*, GridsID > ptr_id_map;

	};

} // end namespace Grids