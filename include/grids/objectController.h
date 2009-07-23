
#pragma once


#include <grids/define.h>
#include <grids/SDLObject.h>

#include <map>
#include <vector>

namespace Grids {
	class Object;
	class Event;
	
	class ObjectController : public SDLObject {
	public:
		ObjectController();
		
		void registerObject( GridsID, Object* );
		GridsID getIDFromPointer( Object* );
		Object* getPointerFromID( GridsID );

		void createObject( GridsID, Event* );
		bool knownObject( GridsID, Event* );	
		void createGenericObject( GridsID, Event* );

		void updateObjectPosition( GridsID, Vec3D );
		void updateObjectRotation( GridsID, Vec3D );
		void updateObjectScale( GridsID, Vec3D );
		void updateObjectAttr( GridsID, Event* );		

	private:		
		std::vector< GridsID > object_ids;
		std::map< GridsID, Object* > id_ptr_map;
		std::map< Object*, GridsID > ptr_id_map;

	};

} // end namespace Grids
