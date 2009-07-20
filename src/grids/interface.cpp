

#include <grids/interface.h>

#include <iostream>

namespace Grids {

	Interface::Interface( ){
		server_address = DEFAULT_SERVER;
		init();
	}

	Interface::Interface(std::string in_server ){
		server_address = in_server;
		init();
	}
		
	void Interface::init(){
		connected = false;

		proto = new Protocol();

		proto->setConnectedCallback( &connectedCallback, this ); 
		proto->setEventCallback( &receiveEvent, this );
	
		proto->connectToNode( server_address.c_str() );

		proto->runEventLoopThreaded();
		std::cout << "Done Init" << std::endl;
	}
	
	Interface::~Interface(){
		proto->closeConnection();		
		proto->stopEventLoopThread();
		
		delete proto;		
	}

	void Interface::receiveEvent(Protocol* in_proto,  Event* in_event, void* self ){

	}

	void Interface::connectedCallback(Protocol* in_proto,  Event* in_event, void* self ){
		((Interface*)self)->connected = true;
	}
	
	GridsID Interface::requestCreateRoom(){

	} 
	
	GridsID Interface::requestCreateObject(GridsID object_room, Value* obj_val ){

	}

	void Interface::requestUpdateObject(GridsID object_id, GridsID object_room, Value* object_attr ){

	}

	std::vector< GridsID > Interface::getRooms(){
		std::vector< GridsID > temp_vec;
		return temp_vec;
	}

	bool Interface::isConnected(){
		return connected;
	}
		
	std::string Interface::getServer(){
		return server_address;
	}


} // end namespace Grids
