
#pragma once

#include <grids/protocol.h>

#define DEFAULT_SERVER "block.hardchats.com"


namespace Grids{

	class Interface {

	public:
		Interface();
		Interface(std::string in_server);
		~Interface();
				
		std::string getServer();

		static void receiveEvent(Protocol*, Event*, void*); // Grids protocol object hooks into this
          static void connectedCallback(Protocol*, Event*, void*);

		bool isConnected();
		
		GridsID requestCreateRoom();
		GridsID requestCreateObject(GridsID object_room, Value* object_attr );		
		void requestUpdateObject(GridsID object_id, GridsID object_room, Value* object_attr );

		std::vector< GridsID > getRooms();

	private:
		void init();
		
		std::string server_address;
		Protocol* proto;
		bool connected;		
	};


} // end namespace Grids
