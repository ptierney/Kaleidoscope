
#pragma once

#include <grids/define.h>
#include <kaleidoscope/define.h>
#include <map>
#include <vector>
#include <QMutex>
#include <QWidget>

namespace Kaleidoscope {
	class Device;
}

namespace Grids {
	class Object;
	class Event;
	
	class ObjectController : QWidget {
		Q_OBJECT
	public:
		ObjectController(Kal::Device*, QWidget* parent = 0);
		
		void registerObject(GridsID, Object*);
		GridsID getIDFromPointer( Object* );
		Object* getPointerFromID( GridsID );
                bool pointerRegistered( Object* );
                bool idRegistered( GridsID );

		void createObject( GridsID, Event* );
		bool knownObject( GridsID, Event* );	
		void createGenericObject( GridsID, Event* );

		void updateObjectPosition( GridsID, Vec3D );
		void updateObjectRotation( GridsID, Vec3D );
		void updateObjectScale( GridsID, Vec3D );
		void updateObjectAttr( GridsID, Event* );		
		
		void detectSelectionThreaded(int, int);

                void deleteObjectFromID( GridsID );
	
	protected:
		void mouseReleasedEvent(QMouseEvent*);

	private:		
		void registerCamera( GridsID, Event* );

		std::vector< GridsID > object_ids;
		std::map< GridsID, Object* > id_ptr_map;
		std::map< Object*, GridsID > ptr_id_map;
                std::map< GridsID, Object* >::iterator id_ptr_iter;
                std::map< Object*, GridsID >::iterator ptr_id_iter;
		
		QMutex map_mutex;

		Kal::Device* d;
	};

} // end namespace Grids
