
#include <grids/object.h>
#include <kaleidoscope/device.h>
#include <grids/objectController.h>
#include <grids/interface.h>
#include <iostream>
#include <QMutexLocker>

namespace Grids {

	Object::Object( Kal::Device* d, Value* in_value ) {
		parent = NULL;

		setID( getIDFromValue( in_value ) );

		d->getObjectController()->registerObject(getID(), this); 

		setInitialPositions( in_value );
		setAttrFromValue( in_value );
	}

	GridsID Object::getID() {
		QMutexLocker lock(&id_mutex);
		return obj_id;
	}
	
	GridsID Object::getRoom() {
		QMuteLocker lock(&id_mutex);
		return obj_room;
	}

	Vec3D Object::getPosition( ){
		Vec3D parents_position = Vec3D();			

		Object* temp_parent = getParent();
				
		if( temp_parent )
			parents_position = temp_parent->getPosition( );
				
		return getLocalPosition() + parents_position;
	}

	Vec3D Object::getScale(  ){
		Vec3D parents_scale = Vec3D(1.f, 1.f, 1.f);
		
		if( parent )
			parents_scale = parent->getScale( );
				
		return getLocalScale() * parents_scale;
	}

	Vec3D Object::getRotation( ){
		Vec3D parents_rot = Vec3D(0.f, 0.f, 0.f);
		
		if(parent)
			parents_rot = parent->getRotation( );
			
		return getLocalRotation() + parents_rot;
	}

	Vec3D Object::getLocalPosition(){
		QMutexLocker lock(&position_mutex);
		return position;
	}
	
	Vec3D Object::getLocalRotation(){
		QMutexLocker lock(&rotation_mutex);
		return rotation;
	}

	Vec3D Object::getLocalScale() {
		QMutexLocker lock(&scale_mutex);
		return scale;
	}
	
	void Object::setLocalPosition( Vec3D pos ){
		QMutexLocker lock(&position_mutex);
		position = pos;
	}
	   
	void Object::setLocalRotation( Vec3D rot ){
		QMutexLocker lock(&rotation_mutex);
		rotation = rot;
	}
	
	void Object::setLocalScale( Vec3D scl ){
		QMutexLocker lock(&scale_mutex);
		scale = scl;
	}

	void Object::updatePosition( Kal::Device* d, Vec3D pos ){
		d->getInterface()->requestUpdatePosition( getID(), pos );
	}

	void Object::updateRotation( Kal::Device* d, Vec3D rot ){
		d->getInterface()->requestUpdateRotation( getID(), rot );
	}

	void Object::updateScale( Kal::Device* d, Vec3D scl ) {
		d->getInterface()->requestUpdateScale( getID(), scl );
	}

	// TODO: This is NOT threadsafe!!
	Object* Object::getParent(){
		QMutexLocker lock(&parent_mutex);		
		return parent;
	}
	
	GridsID Object::getParentID(){
		QMutexLocker lock(&parent_mutex);
		return parent_id;
	}

	void Object::setParent( Object* parent_ptr ){
		QMutexLocker lock(&parent_mutex);
		parent = parent_ptr;
	}
	
	void Object::setParentID( GridsID in_parent_id ){
		QMutexLocker lock(&parent_mutex);
		parent_id = in_parent_id;
	}

	std::vector< Object* > Object::getChildren(){
		QMutexLocker lock(&children_mutex);
		return children;
	}

	GridsID Object::getIDFromValue( Value* val ){
		return (*val)[ "id" ].asString();
	}

	void Object::setInitialPositions( Value* val ){
		if( !( (*val)[ "req" ][ "pos" ].empty() ) ) {
			setLocalPosition( Vec3D( (*val)[ "req" ][ "pos" ][ 0u ].asDouble(),
								(*val)[ "req" ][ "pos" ][ 1u ].asDouble(),
								(*val)[ "req" ][ "pos" ][ 2u ].asDouble() ) );
 		}

		if( !( (*val)[ "req" ][ "rot" ].empty() ) ) {
			setLocalRotation( Vec3D( (*val)[ "req" ][ "rot" ][ 0u ].asDouble(),
								(*val)[ "req" ][ "rot" ][ 1u ].asDouble(),
								(*val)[ "req" ][ "rot" ][ 2u ].asDouble() ) );
		}

		if( !( (*val)[ "req" ][ "scl" ].empty() ) ) {
			setLocalScale( Vec3D( (*val)[ "req" ][ "scl" ][ 0u ].asDouble(),
							  (*val)[ "req" ][ "scl" ][ 1u ].asDouble(),
							  (*val)[ "req" ][ "scl" ][ 2u ].asDouble() ) );
		}
	}

	Value* Object::getAttrFromValue( Value* val ){
		return &((*val)[ "req" ][ "attr" ]);
	}

	Value* Object::getAttr(){
		QMutexLocker lock(&attr_mutex);
		return &attr;
	}
	
	void Object::setAttr( Value* new_attr ){
		QMutexLocker lock(&attr_mutex);
		if(new_attr == NULL)
			attr = Value();
		else
			attr = Value(new_attr);
	}
	
	void Object::setAttrFromValue( Value* new_attr ){
		setAttr( &((*new_attr)[ "req" ][ "attr" ]) );
	}

	//Private:
	
	void Object::setID( GridsID new_id ){
		QMutexLocker lock(&id_mutex);
		obj_id = new_id;
	}
	
	void Object::setRoom( GridsID new_room ){
		QMutexLocker lock(&room_mutex);
		obj_room = new_room;
	}

	void Object::addChild( Object* obj_ptr ){
		QMutexLocker lock(&children_mutex);
		children.push_back( obj_ptr );
	}

	void Object::lockAttr(){ attr_mutex.lock(); }
	void Object::unlockAttr(){ attr_mutex.unlock(); }

	/////////////////////////////////////
	// Position, Rotation, Scale should 
	// not have been in attr! 
	/////////////////////////////////////

	Vec3D Object::getAttrPosition( ){
		QMutexLocker lock(&attr_mutex);
		return Vec3D( attr[ "pos" ][ 0u ].asDouble(),
				    attr[ "pos" ][ 1u ].asDouble(),
				    attr[ "pos" ][ 2u ].asDouble() );
	}

	Vec3D Object::getAttrScale( ){				
		QMutexLocker lock(&attr_mutex);
		return Vec3D( attr[ "scl" ][ 0u ].asDouble(),
				    attr[ "scl" ][ 1u ].asDouble(),
				    attr[ "scl" ][ 2u ].asDouble() );
	}



	Vec3D Object::getAttrRotation( ){		
		QMutexLocker lock(&attr_mutex);
		return Vec3D( attr[ "rot" ][ 0u ].asDouble(),
				    attr[ "rot" ][ 1u ].asDouble(),
				    attr[ "rot" ][ 2u ].asDouble() );
	}

	void Object::setAttrPosition( Vec3D new_position ){
		QMutexLocker lock(&attr_mutex);
		attr[ "pos" ][ 0u ] = new_position.X;
		attr[ "pos" ][ 1u ] = new_position.Y;
		attr[ "pos" ][ 2u ] = new_position.Z;
	}
	
	void Object::setAttrRotation( Vec3D new_rotation ){
		QMutexLocker lock(&attr_mutex);
		attr[ "rot" ][ 0u ] = new_rotation.X;
		attr[ "rot" ][ 1u ] = new_rotation.Y;
		attr[ "rot" ][ 2u ] = new_rotation.Z;
	}

	void Object::setAttrScale( Vec3D new_scale ){
		QMutexLocker lock(&attr_mutex);
		attr[ "scl" ][ 0u ] = new_scale.X;
		attr[ "scl" ][ 1u ] = new_scale.Y;
		attr[ "scl" ][ 2u ] = new_scale.Z;
	}

} // end namespace Grids
