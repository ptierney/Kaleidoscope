
#pragma once


#include <grids/define.h>
#include <grids/objectController.h>
#include <kaleidoscope/define.h>
#include <grids/SDLObject.h>

namespace Kaleidoscope {
	class Device;
}

namespace Grids{

	class Object : public SDLObject {

	public:
		Object( Kal::Device*, Value* );
		
		GridsID getID();
		GridsID getRoom();

		// Gets the absolute Position, including parents
		virtual Vec3D getPosition( );
		virtual Vec3D getScale( );
		virtual Vec3D getRotation( );

		virtual Vec3D getLocalPosition();
		virtual Vec3D getLocalRotation();
		virtual Vec3D getLocalScale();		
		
		virtual void updatePosition( Kal::Device*, Vec3D );
		virtual void updateScale( Kal::Device*, Vec3D );
		virtual void updateRotation( Kal::Device*, Vec3D );

		// attr is a Grids::Value that holds all the information
		// specific to the particular object.
		// This information should exist in a shared state between the
		// computers on the network.
		Value* getAttr();
		void lockAttr();
		void unlockAttr();
		
		void addChild( Object* obj_ptr );
		
		friend class ObjectController;

	protected:
		Object* getParent();
		GridsID getParentID();
		void setRoom( GridsID );
		void setParent(Object*);
		void setParentID( GridsID );
		Value getAttrFromValue( Value* );
		GridsID getIDFromValue( Value* );
		
		std::vector< Object* > children;

		Value attr;

 		// These functions are called by the ObjectController
		// When it receives a message GRIDS_UPDATE_OBJECT
		void setLocalPosition( Vec3D );
		void setLocalRotation( Vec3D );
		void setLocalScale( Vec3D );

	private:
		void setAttr( Value* );
		void setAttrFromValue( Value* );
		void setInitialPositions( Value* );
		
		void setID( GridsID );

		Object* parent;
		GridsID parent_id;
	 
		GridsID obj_id;
		GridsID obj_room;
		
		Vec3D position;
		Vec3D rotation;
		Vec3D scale;
		
		SDL_mutex* position_mutex;
		SDL_mutex* rotation_mutex;
		SDL_mutex* scale_mutex;		
		SDL_mutex* children_mutex;		

		////////////////////////
		// These will be deleted
		////////////////////////
		void setAttrPosition( Vec3D );
		void setAttrRotation( Vec3D );
		void setAttrScale( Vec3D );

		virtual Vec3D getAttrPosition(  );
		virtual Vec3D getAttrScale( );
		virtual Vec3D getAttrRotation( );
	};

} // end namespace Grids

