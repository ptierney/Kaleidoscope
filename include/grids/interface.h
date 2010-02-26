
#pragma once

#include <kaleidoscope/define.h>
#include <grids/define.h>

#include <QObject>
#include <QMutex>

namespace Kaleidoscope {
    class Device;
    class Room;
}

namespace Grids{

    class Protocol;
    class Event;


    class Interface : public QObject {
        Q_OBJECT
    public:
        Interface(Kaleidoscope::Device*, QObject* parent = 0);
        Interface(Kaleidoscope::Device*, std::string in_server, QObject* parent = 0);
        ~Interface();

        std::string getServer();

        static void receiveEvent(Protocol*, Event*, void*); // Grids protocol object hooks into this
        static void connectedCallback(Protocol*, Event*, void*);

        bool isConnected();

        void requestCreateRoom();
        void createMyRoom();

        /* These functions are used to create objects in the world,
		   with either a default position, a specific position, or position + rotation + scale 
		   These functions create a GridsID for the object, and returns it so you know how to find the 
		   object when it is finally created */
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

        void setMyRoom(GridsID);

        /* Read events from the protocol event queue. */
        void collectEvents();

        friend class Kaleidoscope::Device;

        void printVal(Value*);

    public slots:
        void parseEvent(Event*);

        /* Called when protocol receives the correct init string */
        void protocolInitiated(Event*);

        /* Slot to pass the raw data received from Grids */
        void rawReceive(QString);
	
    signals:
        void error(int, QString);
        void notice(int, QString);
        void gridsConnectionEstablished();
        void myRoomCreated(GridsID);

    private:
        void init();
        void setConnected( bool );
        //void registerNewRoom( Kaleidoscope::Room* );
        void registerCreateRoom(Event*);
        void registerNewRoom(GridsID);
        void receiveRoomList(Event*);

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

        Kaleidoscope::Device* d;
    };


} // end namespace Grids
