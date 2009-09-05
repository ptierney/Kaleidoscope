
#pragma once

#include <grids/protocol.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>

namespace Kaleidoscope {
	class Device;
}

namespace Grids{

	class Interface : QObject {
		Q_OBJECT
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
		GridsID requestCreateObject(Value* object_attr, Vec3D pos );		
		GridsID requestCreateObject(Value* object_attr, Vec3D pos, Vec3D rot, Vec3D scl );		

		void requestUpdateAttr(GridsID object_id, Value* object_attr );
		void requestUpdatePosition(GridsID object_id, Vec3D new_pos );
		void requestUpdateRotation(GridsID object_id, Vec3D new_pos );
		void requestUpdateScale(GridsID object_id, Vec3D new_pos );
		
		GridsID getMyRoom();
		// Known rooms are rooms that I've created or 
		// have been messaged as being created.
		// Server rooms are the rooms on the server,
		// accessed with GRIDS_LIST_ROOMS 
		std::vector< GridsID > getKnownRooms();
		std::vector< GridsID > getServerRooms();
		void requestAllRooms();
		
		friend class Kaleidoscope::Device;

	public slots:
		void parseEvent(Event*);
		void protocolInitiated(Event*);
	
	signals:
		void error(int, QString);
		void notice(int, QString);

	private:
		void init();
		void parseEvent(Event*);
		void setConnected( bool );
		void registerNewRoom( Kal::Room* );	
			
		std::vector< GridsID > known_rooms;		
		std::vector< GridsID > server_rooms;

		std::string server_address;
		Protocol* proto;
		bool connected;		
		
		GridsID my_room;
		
		QMutex my_room_mutex;	
		QMutex known_rooms_mutex;
		QMutex connected_mutex;
		QMutex parse_event_mutex;

		Kal::Device* d;
	};


} // end namespace Grids
