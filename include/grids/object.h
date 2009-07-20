
#pragma once

#include <grids/SDLObject.h>
#include <grids/define.h>
#include <kaleidoscope/define.h>

namespace Kaleidoscope {
	class Device;
}

namespace Grids{

	class Object : public SDLObject {

	public:
		Object( Kal::Device*, Value* );
		
		GridsID getID();
		GridsID getRoom();

		virtual Vec3D getPosition(  );
		virtual Vec3D getScale( );
		virtual Vec3D getRotation( );

		void setAttrPosition( Vec3D );
		void setAttrRotation( Vec3D );
		void setAttrScale( Vec3D );

		Value* getAttr();
	protected:

		virtual Vec3D getAttrPosition(  );
		virtual Vec3D getAttrScale( );
		virtual Vec3D getAttrRotation( );

		Object* getParent();
		GridsID getParentID();
		void setRoom( GridsID );
		void setParent(Object*);
		void setParentID( GridsID );
		
		std::vector< Object* > children;

	private:
		GridsID getIDFromValue( Value* );
		Value getAttrFromValue( Value* );
		void setAttr( Value* );
		void setAttrFromValue( Value* );
		
		void setID( GridsID );

		Object* parent;
		GridsID parent_id;
	 
		GridsID obj_id;
		GridsID obj_room;
		
		Value attr;
	};

} // end namespace Grids

