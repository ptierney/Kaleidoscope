
#include <grids/interface.h>
#include <grids/utility.h>
#include <grids/objectController.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/noticeWindow.h>
#include <iostream>
#include <algorithm>
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
        connect(this, SIGNAL(gridsConnectionEstablished()),
                d, SLOT(gridsConnectionEstablished()));

        connect(this, SIGNAL(myRoomCreated(GridsID)),
                d, SLOT(myRoomCreated(GridsID)));

        connected = 0;
        proto = new Protocol(d->getMyID(), this);
        d->getNoticeWindow()->write(0, tr("Created Protocol"));

        connect(proto, SIGNAL(protocolInitiated(Event*)),
                this, SLOT(protocolInitiated(Event*)));
        connect(proto, SIGNAL(receiveEvent(Event*)),
                this, SLOT(parseEvent(Event*)));
        connect(proto, SIGNAL( rawData(QString)),
                this, SLOT( rawReceive(QString)));

        proto->connectToNode( server_address.c_str() );
        d->getNoticeWindow()->write(7, tr("Connected to Node"));

        proto->runEventLoopThreaded();
        d->getNoticeWindow()->write(0, tr("Running Event Loop"));
    }

    Interface::~Interface(){
        proto->closeConnection();
        proto->stopEventLoopThread();
    }

    void Interface::rawReceive(QString raw_data){
        d->getNoticeWindow()->write(1, tr("rec>> ") + raw_data);
    }

    void Interface::protocolInitiated(Event* in_event){
        setConnected(1);
        emit gridsConnectionEstablished();
    }

    void Interface::parseEvent( Event* evt ){
        QMutexLocker lock(&parse_event_mutex);

        std::string event_type = evt->getEventType();
        Grids::GridsID object_id = evt->getID();

        if( event_type == GRIDS_CREATE_ROOM ){
            registerNewRoom( new Kal::Room( d, evt->getArgsPtr() ) );
        } else if( event_type == GRIDS_LIST_ROOMS ) {
            receiveRoomList( evt );
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
        }
    }

    // END: Protocol thread functions

    void Interface::requestCreateRoom(){
        Grids::Value msg;
        msg["_method"] = GRIDS_CREATE_ROOM;

        proto->sendRequest( GRIDS_CREATE_ROOM, &msg );
    }

    void Interface::createMyRoom() {
        if(!(getMyRoom().empty()))
            return;

        requestCreateRoom();
    }

    void Interface::setMyRoom(GridsID new_room) {
        my_room = new_room;
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
        Grids::Value* msg = new Grids::Value();
        (*msg)["_method"] = GRIDS_LIST_ROOMS;

        proto->sendRequest( GRIDS_LIST_ROOMS, msg );
    }

    GridsID Interface::getMyRoom(){
        QMutexLocker lock(&my_room_mutex);
        return my_room;
    }

    void Interface::registerNewRoom( Kal::Room* rm ){
        GridsID rm_id = rm->getID();
        bool my_room_created = 0;

        // Make my room the first available room
        if( my_room.empty() ){
            QMutexLocker my_room_lock(&my_room_mutex);
            my_room = rm_id;
            my_room_created = 1;
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

        if(my_room_created)
            emit myRoomCreated(rm_id);
    }

    void Interface::receiveRoomList(Grids::Event* evt) {
        Value* args = evt->getArgsPtr();
        int num_rooms = (*args)["rooms"].size();

        if( num_rooms == 0 )
            return;

        std::vector<GridsID>::iterator result;

        for( unsigned int i = 0; i < num_rooms; i++) {
            GridsID room_id = (*args)["rooms"][i].asString();
            result = find(known_rooms.begin(), known_rooms.end(), room_id );

            if( result == known_rooms.end() ) { /* did not find room. */
                /* Create fake Grids::Value* */
                Value* temp_val = new Value();
                (*temp_val)["id"] = room_id;

                registerNewRoom(new Kal::Room(d, temp_val));

                delete temp_val;
            }
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
