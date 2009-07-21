
#pragma once

#include <grids/protocol.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/room.h>

#define DEFAULT_SERVER "block.hardchats.com"

namespace Kaleidoscope {
	class Device;
}

namespace Grids{

	class Interface {

	public:
		Interface(Kal::Device*);
		Interface(Kal::Device*, std::string in_server);
		~Interface();
				
		std::string getServer();

		static void receiveEvent(Protocol*, Event*, void*); // Grids protocol object hooks into this
          static void connectedCallback(Protocol*, Event*, void*);

		bool isConnected();
		
		void requestCreateRoom();
		GridsID createMyRoom( int timeout );

		GridsID requestCreateObject(Value* object_attr );		
		void requestUpdateObject(GridsID object_id, Value* object_attr );

		GridsID getMyRoom();
		std::vector< GridsID > getKnownRooms();

	private:
		void init();
		void setupMutexes();
		void parseEvent(Event*);
		void setConnected( bool );
		void registerNewRoom( Kal::Room* );		

		std::vector< GridsID > known_rooms;		

		std::string server_address;
		Protocol* proto;
		bool connected;		
		
		GridsID my_room;
		
		SDL_mutex* my_room_mutex;	
		SDL_mutex* known_rooms_mutex;
		SDL_mutex* connected_mutex;
		SDL_mutex* parse_event_mutex;

		Kal::Device* d;
	};


} // end namespace Grids
