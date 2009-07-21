

#include <grids/interface.h>
#include <grids/utility.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>

#include <iostream>

namespace Grids {

	Interface::Interface( Kal::Device* _d ){
		server_address = DEFAULT_SERVER;
		d = _d;

		init();
	}

	Interface::Interface(Kal::Device* _d, std::string in_server ){
		server_address = in_server;
		d = _d;

		init();
	}
		
	void Interface::init(){
		connected = false;
		setupMutexes();

		proto = new Protocol();

		proto->setConnectedCallback( &connectedCallback, this ); 
		proto->setEventCallback( &receiveEvent, this );
	
		proto->connectToNode( server_address.c_str() );

		proto->runEventLoopThreaded();

		while( !isConnected() ){
			sleep( 1 );
		}

	}
	
	Interface::~Interface(){
		proto->closeConnection();		
		proto->stopEventLoopThread();
		
		delete proto;		
	}
	
	void Interface::setupMutexes(){
		connected_mutex = SDL_CreateMutex();
		parse_event_mutex = SDL_CreateMutex();
		my_room_mutex = SDL_CreateMutex();
		known_rooms_mutex = SDL_CreateMutex();
	}

	// START: Protocol thread functions
	void Interface::receiveEvent(Protocol* in_proto,  Event* in_event, void* self ){
		((Interface*)self)->parseEvent( in_event );
	}

	void Interface::connectedCallback(Protocol* in_proto,  Event* in_event, void* self ){
		((Interface*)self)->setConnected( 1 );
	}
	
	void Interface::parseEvent( Event* evt ){
		SDL_LockMutex( parse_event_mutex );

		std::string event_type = evt->getEventType();
		Grids::GridsID object_id = evt->getArgs()[ "id" ].asString();

		if( event_type == GRIDS_CREATE_ROOM ){
			registerNewRoom( new Kal::Room( d, evt->getArgsPtr()) );						
		} else if( event_type == GRIDS_CREATE_OBJECT ){
			
		} else if( event_type == GRIDS_UPDATE_OBJECT ){

		}

		SDL_UnlockMutex( parse_event_mutex );
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
			sleep( 1 );
			wait_secs++;
		}
		
		return std::string();
	}

	// By default this creates an object in your room
	// Overriding object creation to another room, 	
	GridsID Interface::requestCreateObject( Value* attr ){
		GridsID new_obj_id = d->getGridsUtility()->getNewUUID();
		
		Grids::Value* msg = new Grids::Value();
		(*msg)[ "_method" ] = GRIDS_CREATE_OBJECT;
		(*msg)[ "room_id" ] = getMyRoom();
		(*msg)[ "id" ] = new_obj_id;
		(*msg)[ "attr" ] = attr;

		proto->sendRequest( GRIDS_CREATE_OBJECT, msg );
		
		delete msg;
		
		return new_obj_id;
	}

	void Interface::requestUpdateObject(GridsID object_id, Value* object_attr ){

	}

	std::vector< GridsID > Interface::getKnownRooms(){
		return known_rooms;
	}

	GridsID Interface::getMyRoom(){ 
		std::string temp_room; 

		SDL_LockMutex( my_room_mutex );
		temp_room = my_room;
		SDL_UnlockMutex( my_room_mutex );

		return temp_room;
	}

	void Interface::registerNewRoom( Kal::Room* rm ){
		GridsID rm_id = rm->getID();
		
		if( my_room.empty() ){
			SDL_LockMutex( my_room_mutex );
			my_room = rm_id;
			SDL_UnlockMutex( my_room_mutex );
		}
		
		bool in_vector = 0;
		
		for( int i = 0; i < known_rooms.size(); i++ ){
			if( known_rooms[i] == rm_id )
				in_vector = 1;
		}

		if( !in_vector){
			SDL_LockMutex( known_rooms_mutex );
			known_rooms.push_back( rm_id );
			SDL_UnlockMutex( known_rooms_mutex );
		}
	}

	bool Interface::isConnected(){
		bool temp_con;

		SDL_LockMutex( connected_mutex );
		temp_con = connected;
		SDL_UnlockMutex( connected_mutex );
		
		return temp_con;
	}
		
	std::string Interface::getServer(){
		return server_address;
	}
	
	void Interface::setConnected( bool con_stat ){
		SDL_LockMutex( connected_mutex );
		connected = con_stat; 
		SDL_UnlockMutex( connected_mutex );
	}


} // end namespace Grids
