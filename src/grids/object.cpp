
#include <grids/object.h>
#include <kaleidoscope/device.h>
#include <grids/objectController.h>

namespace Grids {

	Object::Object( Kal::Device* d, Value* in_value ) {
		setID( getIDFromValue( in_value ) );

		d->getObjectController()->registerObject( getID(), this ); 
	
		setAttrFromValue( in_value ); 
	}

	GridsID Object::getID() {
		GridsID temp_id;
		
		lock();
		temp_id = obj_id;
		unlock();

		return temp_id;
	}
	
	GridsID Object::getRoom() {
		GridsID temp_id;
		
		lock();
		temp_id = obj_room;
		unlock();

		return temp_id;
	}

	Vec3D Object::getPosition( ){
		Vec3D parents_position = Vec3D();
	
		Object* temp_parent = getParent();
				
		if( temp_parent )
			parents_position = temp_parent->getPosition( );
				
		return getAttrPosition( ) + parents_position;
	}

	Vec3D Object::getAttrPosition( ){		
		Vec3D temp_position;

		lock();
		temp_position = Vec3D( attr[ "pos" ][ 0u ].asDouble(),
						   attr[ "pos" ][ 1u ].asDouble(),
						   attr[ "pos" ][ 2u ].asDouble() );
		unlock();
	
		return temp_position;
	}

	Vec3D Object::getScale(  ){
		Vec3D parents_scale = Vec3D(1.0f, 1.0f, 1.0f );
		
		
		if( parent )
			parents_scale = parent->getScale( );
				
		return getAttrScale( ) * parents_scale;
	}

	Vec3D Object::getAttrScale( ){				
		Vec3D temp_scale;

		lock();
		return Vec3D( attr[ "scl" ][ 0u ].asDouble(),
				    attr[ "scl" ][ 1u ].asDouble(),
				    attr[ "scl" ][ 2u ].asDouble() );
		unlock();
	
		return temp_scale;
	}


	Vec3D Object::getRotation( ){
		Vec3D parents_rot = Vec3D(0.0f, 0.0f, 0.0f );
		
		if( parent )
			parents_rot = parent->getRotation( );
			
		return getAttrRotation( ) + parents_rot;
	}

	Vec3D Object::getAttrRotation( ){		
		Vec3D temp_rotation;

		lock();
		temp_rotation = Vec3D( attr[ "rot" ][ 0u ].asDouble(),
						   attr[ "rot" ][ 1u ].asDouble(),
						   attr[ "rot" ][ 2u ].asDouble() );
		unlock();
	
		return temp_rotation;
	}

	void Object::setAttrPosition( Vec3D new_position ){
		lock();
		attr[ "pos" ][ 0u ] = new_position.X;
		attr[ "pos" ][ 1u ] = new_position.Y;
		attr[ "pos" ][ 2u ] = new_position.Z;
		unlock();
	}
	
	void Object::setAttrRotation( Vec3D new_rotation ){
		lock();
		attr[ "rot" ][ 0u ] = new_rotation.X;
		attr[ "rot" ][ 1u ] = new_rotation.Y;
		attr[ "rot" ][ 2u ] = new_rotation.Z;
		unlock();
	}

	void Object::setAttrScale( Vec3D new_scale ){
		lock();
		attr[ "scl" ][ 0u ] = new_scale.X;
		attr[ "scl" ][ 1u ] = new_scale.Y;
		attr[ "scl" ][ 2u ] = new_scale.Z;
		unlock();
	}

	Object* Object::getParent(){
		Object* temp_parent;

		lock();
		temp_parent = parent;
		unlock();
		
		return temp_parent;
	}
	
	GridsID Object::getParentID(){
		GridsID temp_parent;
		lock();
		temp_parent = parent_id;
		unlock();
		
		return temp_parent;
	}

	void Object::setParent( Object* parent_ptr ){
		lock();
		parent = parent_ptr;		
		unlock();
	}
	
	void Object::setParentID( GridsID in_parent_id ){
		lock();
		parent_id = in_parent_id;
		unlock();
	}

	GridsID Object::getIDFromValue( Value* val ){
		return (*val)[ "id" ].asString();
	}

	Value* getAttrFromValue( Value* val ){
		return &((*val)[ "req" ][ "attr" ]);
	}

	Value* Object::getAttr(){
		return &attr;
	}
	
	void Object::setAttr( Value* new_attr ){
		attr = Value( new_attr );
	}
	
	void Object::setAttrFromValue( Value* new_attr ){
		setAttr( &((*new_attr)[ "req" ][ "attr" ]) );
	}

	//Private:
	
	void Object::setID( GridsID new_id ){
		lock();
		obj_id = new_id;
		unlock();
	}
	
	void Object::setRoom( GridsID new_room ){
		lock();
		obj_room = new_room;
		unlock();
	}







} // end namespace Grids
