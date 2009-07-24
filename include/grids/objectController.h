
#pragma once


#include <grids/define.h>
#include <grids/SDLObject.h>
#include <kaleidoscope/define.h>

#include <map>
#include <vector>

namespace Kaleidoscope {
	class Device;
}

namespace Grids {
	class Object;
	class Event;
	
	class ObjectController : public SDLObject {
	public:
		ObjectController(Kal::Device*);
		
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
		void registerCamera( GridsID, Event* );

		std::vector< GridsID > object_ids;
		std::map< GridsID, Object* > id_ptr_map;
		std::map< Object*, GridsID > ptr_id_map;

		Kal::Device* d;
	};

} // end namespace Grids
