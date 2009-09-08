

#include <grids/interface.h>
#include <grids/utility.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>
#include <iostream>
#include <QTcpSocket>
#include <QString>

namespace Grids {

	Interface::Interface(Kal::Device* _d, QObject* parent)
		: QObject(parent) {
		server_address = DEFAULT_SERVER;
 		d = _d;
		
		init();
	}

	Interface::Interface(Kal::Device* _d, std::string in_server, QObject* parent)
		: QObject(parent) {
		server_address = in_server;
		d = _d;
		
		init();
	}

	// Spawns new Protocol and network listening thread
	// This call will hang if there is no connection to the internet
	void Interface::init(){				
		connected = 0;
		proto = new Protocol();
		
		connect(proto, SIGNAL(protocolInitiated(Event*)),
			   this, SLOT(protocolInitiated(Event*)));
		connect(proto, SIGNAL(receiveEvent(Event*)),
			   this, SLOT(parseEvent(Event*)));
	
		proto->connectToNode( server_address.c_str() );
		proto->runEventLoopThreaded();

		while( !isConnected() ){
			emit notice(4, tr("Connecting to server."));
			sleep( 1 );
		}
	}
	
	Interface::~Interface(){
		proto->closeConnection();		
		proto->stopEventLoopThread();
		
		delete proto;		
	}
	
	void Interface::protocolInitiated(Event* in_event){
		setConnected(1);
	}
	
	void Interface::parseEvent( Event* evt ){
		QMutexLocker(&parse_event_mutex);

		std::string event_type = evt->getEventType();
		Grids::GridsID object_id = evt->getID();

		if( event_type == GRIDS_CREATE_ROOM ){
			registerNewRoom( new Kal::Room( d, evt->getArgsPtr() ) );						
		} else if( event_type == GRIDS_CREATE_OBJECT ){
			d->getObjectController()->createObject( object_id, evt );			
		} else if( event_type == GRIDS_UPDATE_OBJECT ){
			if( evt->hasPosition() )
				d->getObjectController()->updateObjectPosition( object_id, evt->getPosition() );
			if( evt->hasRotation() )
				d->getObjectController()->updateObjectRotation( object_id, evt->getRotation() );
			if( evt->hasScale() )
				d->getObjectController()->updateObjectScale( object_id, evt->getScale() ); 
			if( evt->hasAttr() )
				d->getObjectController()->updateObjectAttr( object_id, evt );
		} else if( event_type == GRIDS_LIST_ROOMS ) {
			// d->getUtility()->parseListRooms( evt );
		}
	}
	
	// END: Protocol thread functions
	
	void Interface::requestCreateRoom(){
		Grids::Value msg;
		msg["_method"] = GRIDS_CREATE_ROOM;
				
		proto->sendRequest( GRIDS_CREATE_ROOM, &msg );			
	} 
	
	GridsID Interface::createMyRoom( int timeout ) {
		if( !(getMyRoom().empty()) ){
			return getMyRoom();
		}
		
		requestCreateRoom();
		
		int wait_secs = 0;
		
		while( wait_secs < timeout ){
			if( !(getMyRoom().empty()) )
				return getMyRoom();
			emit notice(4, tr("Attempting to create room."));
			sleep( 1 );
			wait_secs++;
		}
		
		return std::string();
	}

	// By default this creates an object in your room
	// Overriding object creation to another room, 	
	GridsID Interface::requestCreateObject( Value* attr ){
		return requestCreateObject( attr, Vec3D( 0.0f, 0.0f, 0.0f ) );
	}
	
	GridsID Interface::requestCreateObject( Value* attr, Vec3D pos ){
		return requestCreateObject( attr, pos, Vec3D( 0.0f, 0.0f, 0.0f ), Vec3D( 1.0f, 1.0f, 1.0f ) );
	}

	GridsID Interface::requestCreateObject( Value* attr, Vec3D pos, Vec3D rot, Vec3D scl ){
		GridsID new_obj_id = d->getGridsUtility()->getNewUUID();
		
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_CREATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = new_obj_id;
		(*msg)[ "pos" ][ 0u ] = pos.X;
		(*msg)[ "pos" ][ 1u ] = pos.Y;
		(*msg)[ "pos" ][ 2u ] = pos.Z;
		(*msg)[ "rot" ][ 0u ] = rot.X;
		(*msg)[ "rot" ][ 1u ] = rot.Y;
		(*msg)[ "rot" ][ 2u ] = rot.Z;
		(*msg)[ "scl" ][ 0u ] = scl.X;
		(*msg)[ "scl" ][ 1u ] = scl.Y;
		(*msg)[ "scl" ][ 2u ] = scl.Z;
		(*msg)[ "attr" ] = *attr;

		proto->sendRequest( GRIDS_CREATE_OBJECT, msg );
		
		delete msg;
		
		return new_obj_id;
	}

	void Interface::requestUpdateAttr(GridsID object_id, Value* object_attr ){
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = object_id;

		(*msg)[ "attr" ] = object_attr;
	
		proto->sendRequest( GRIDS_UPDATE_OBJECT, msg );
	}
	   
	void Interface::requestUpdatePosition(GridsID object_id, Vec3D new_pos ){
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = object_id;

		(*msg)[ "pos" ][ 0u ] = new_pos.X;
		(*msg)[ "pos" ][ 1u ] = new_pos.Y;
		(*msg)[ "pos" ][ 2u ] = new_pos.Z;
	
		proto->sendRequest( GRIDS_UPDATE_OBJECT, msg );
	}

	void Interface::requestUpdateRotation(GridsID object_id, Vec3D new_rot ){
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = object_id;

		(*msg)[ "rot" ][ 0u ] = new_rot.X;
		(*msg)[ "rot" ][ 1u ] = new_rot.Y;
		(*msg)[ "rot" ][ 2u ] = new_rot.Z;
	
		proto->sendRequest( GRIDS_UPDATE_OBJECT, msg );
	}
	void Interface::requestUpdateScale(GridsID object_id, Vec3D new_scl ){
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = object_id;

		(*msg)[ "scl" ][ 0u ] = new_scl.X;
		(*msg)[ "scl" ][ 1u ] = new_scl.Y;
		(*msg)[ "scl" ][ 2u ] = new_scl.Z;
	
		proto->sendRequest( GRIDS_UPDATE_OBJECT, msg );
	}

	std::vector< GridsID > Interface::getKnownRooms(){
		return known_rooms;
	}

	void Interface::requestAllRooms(){

	}	

	GridsID Interface::getMyRoom(){
		QMutexLocker lock(&my_room_mutex);
		return my_room;
	}

	void Interface::registerNewRoom( Kal::Room* rm ){
		GridsID rm_id = rm->getID();
		
		// Make my room the first available room
		if( my_room.empty() ){
			QMutexLocker my_room_lock(&my_room_mutex);
			my_room = rm_id;
		}
		
		bool in_vector = 0;
		
		for( int i = 0; i < known_rooms.size(); i++ ){
			if( known_rooms[i] == rm_id )
				in_vector = 1;
		}

		if( !in_vector){
			QMutexLocker known_lock(&known_rooms_mutex);
			known_rooms.push_back( rm_id );
		}
	}
	

	bool Interface::isConnected(){
		QMutexLocker lock(&connected_mutex);
		return connected;
	}
		
	std::string Interface::getServer(){
		return server_address;
	}
	
	void Interface::setConnected( bool con_stat ){
		QMutexLocker lock(&connected_mutex);
		connected = con_stat;
	}


} // end namespace Grids
