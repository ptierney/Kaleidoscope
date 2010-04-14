
#include <grids/interface.h>
#include <grids/utility.h>
#include <grids/objectController.h>
#include <grids/protocol.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>

#include <iostream>
#include <algorithm>
#include <QTcpSocket>
#include <QString>

namespace Grids {

  Interface::Interface(Kaleidoscope::Device* _d, QObject* parent)
    : QObject(parent) {
    server_address = DEFAULT_SERVER;
    d = _d;

    init();
  }

  Interface::Interface(Kaleidoscope::Device* _d, std::string in_server, QObject* parent)
    : QObject(parent) {
    server_address = in_server;
    d = _d;

    init();
  }

  // Spawns new Protocol and network listening thread
  // This call will hang if there is no connection to the internet
  void Interface::init(){
    /* This isn't threadafe, so it's got to go.
        connect(this, SIGNAL(gridsConnectionEstablished()),
                d, SLOT(gridsConnectionEstablished()));
        */
    /*This doesn't do shit, so it's got to go.
    connect(this, SIGNAL(myRoomCreated(GridsID)),
            d, SLOT(myRoomCreated(GridsID)));
            */

    connected = 0;
    proto = new Protocol(d->getMyID(), this);

    connect(proto, SIGNAL(protocolInitiated(Event*)),
            this, SLOT(protocolInitiated(Event*)));
    connect(proto, SIGNAL(receiveEvent(Event*)),
            this, SLOT(parseEvent(Event*)));
    /* Events should not be send directly through signals / slots,
           all events must go through the Event-queue
        connect(proto, SIGNAL( rawData(QString)),
                this, SLOT( rawReceive(QString)));
        */

    proto->connectToNode( server_address.c_str() );  
  }

  Interface::~Interface(){
    proto->closeConnection();
    delete proto;
  }

  void Interface::rawReceive(QString /*raw_data*/){
    //d->getNoticeWindow()->write(1, tr("rec>> ") + raw_data);
  }

  void Interface::protocolInitiated(Event* /*in_event*/){
    setConnected(1);
    /*emit gridsConnectionEstablished();*/
  }

  void Interface::collectEvents() {

    EventQueue& temp_queue = proto->getEvents();

    while(!temp_queue.empty()){
      Event* temp_event = temp_queue.front();
      parseEvent(temp_event);

      delete temp_event;
      temp_queue.pop();
    }
  }

  void Interface::parseEvent( Event* evt ){
    /* Check for the success bounceback, though Room.List should be kept. */
    const Value& args = evt->getArgs();
    std::string event_type = evt->getEventType();
    Grids::GridsID object_id = evt->getID();

    GridsID request_id = args["request_id"].asString();

    // In an astounding act of ghetto-ness, I've resorted to tagging each event with a unique
    // ID, and checking for duplicates. The problem is probaly with flushing the socket
    // Oh well.
    // Updated Grids Node 4.14.10, fixed bug, though keeping this in case it pops up again
    /*
    if(request_id.empty() == false){
      if(duplicate_map_[request_id] == 0){
        duplicate_map_[request_id] = 1;
      } else {
        // events get cleaned up in collectEvents
        std::cerr << "Fuck" << std::endl;
        return;
      }
    }
    */

    if(args["success"].empty() == false){
      if(event_type != GRIDS_LIST_ROOMS &&
         event_type != GRIDS_CREATE_ROOM ){
        return;
      }
    }

    if( event_type == GRIDS_CREATE_ROOM ){
      registerCreateRoom(evt);
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

  void Interface::requestCreateRoom() {
    Grids::Value msg;
    msg["_method"] = GRIDS_CREATE_ROOM;

    proto->sendRequest( GRIDS_CREATE_ROOM, &msg, LOCAL );
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
    (*msg)[ "request_id" ] = d->getGridsUtility()->getNewUUID();

    proto->sendRequest( GRIDS_CREATE_OBJECT, msg, BROADCAST );

    delete msg;

    return new_obj_id;
  }

  void Interface::requestUpdateAttr(GridsID object_id, Value* object_attr ){
    Grids::Value* msg = new Grids::Value();
    (*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
    (*msg)[ "room_id" ] = getMyRoom();
    (*msg)[ "id" ] = object_id;
    // For debuging purposes
    (*msg)[ "request_id" ] = d->getGridsUtility()->getNewUUID();

    (*msg)[ "attr" ] = *object_attr;

    proto->sendRequest( GRIDS_UPDATE_OBJECT, msg, BROADCAST );
  }

  void Interface::requestUpdatePosition(GridsID object_id, Vec3D new_pos ){
    Grids::Value* msg = new Grids::Value();
    (*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
    (*msg)[ "room_id" ] = getMyRoom();
    (*msg)[ "id" ] = object_id;

    (*msg)[ "pos" ][ 0u ] = new_pos.X;
    (*msg)[ "pos" ][ 1u ] = new_pos.Y;
    (*msg)[ "pos" ][ 2u ] = new_pos.Z;

    proto->sendRequest( GRIDS_UPDATE_OBJECT, msg, BROADCAST );
  }

  void Interface::requestUpdateRotation(GridsID object_id, Vec3D new_rot ){
    Grids::Value* msg = new Grids::Value();
    (*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
    (*msg)[ "room_id" ] = getMyRoom();
    (*msg)[ "id" ] = object_id;

    (*msg)[ "rot" ][ 0u ] = new_rot.X;
    (*msg)[ "rot" ][ 1u ] = new_rot.Y;
    (*msg)[ "rot" ][ 2u ] = new_rot.Z;

    proto->sendRequest( GRIDS_UPDATE_OBJECT, msg, BROADCAST );
  }
  void Interface::requestUpdateScale(GridsID object_id, Vec3D new_scl ){
    Grids::Value* msg = new Grids::Value();
    (*msg)[ "_method" ] = GRIDS_UPDATE_OBJECT;
    (*msg)[ "room_id" ] = getMyRoom();
    (*msg)[ "id" ] = object_id;

    (*msg)[ "scl" ][ 0u ] = new_scl.X;
    (*msg)[ "scl" ][ 1u ] = new_scl.Y;
    (*msg)[ "scl" ][ 2u ] = new_scl.Z;

    proto->sendRequest( GRIDS_UPDATE_OBJECT, msg, BROADCAST );
  }

  const std::vector<GridsID>& Interface::getKnownRooms(){
    return known_rooms;
  }

  const std::vector<GridsID>& Interface::getServerRooms(){
    return server_rooms;
  }

  void Interface::requestAllRooms(){
    Grids::Value* msg = new Grids::Value();
    (*msg)["_method"] = GRIDS_LIST_ROOMS;

    proto->sendRequest( GRIDS_LIST_ROOMS, msg, LOCAL );
  }

  GridsID Interface::getMyRoom(){
    return my_room;
  }

  void Interface::registerCreateRoom(Event* evt) {
    /* register new room evt->["id"] */
    GridsID new_room_id = (*(evt->getArgsPtr()))["id"].asString();
    //d->getNoticeWindow()->write(5, tr("Room created with id = ") + tr(new_room_id.c_str()));
    registerNewRoom( new_room_id );
  }

  void Interface::registerNewRoom(GridsID new_id){
    GridsID rm_id = new_id;
    bool my_room_created = 0;
    //std::cerr << "Registering new room" << new_id << std::endl;

    // Make my room the first available room
    if( my_room.empty() ){
      my_room = rm_id;
      my_room_created = 1;
      //std::cerr << "Room set to " << my_room << std::endl;
    }

    bool room_known = false;

    for( std::vector<GridsID>::const_iterator it = known_rooms.begin(); it != known_rooms.end(); ++it ){
      if( *it == rm_id )
        room_known = true;
    }

    if( !room_known)
      known_rooms.push_back( rm_id );

    if(my_room_created)
      emit myRoomCreated(rm_id);
  }

  void Interface::receiveRoomList(Grids::Event* evt) {
    Value* args = evt->getArgsPtr();
    unsigned int num_rooms = (*args)["rooms"].size();

    if( num_rooms == 0u ){
      createMyRoom();
      return;
    }

    std::vector<GridsID>::iterator result;

    for( unsigned int i = 0u; i < num_rooms; i++) {
      GridsID room_id = (*args)["rooms"][i].asString();
      result = find(known_rooms.begin(), known_rooms.end(), room_id );

      if( result == known_rooms.end() ) { /* did not find room. */
        registerNewRoom(room_id);
      }
    }
  }

  void Interface::flushProtocol() {
    proto->protocolFlush();
  }

  void Interface::printVal(Value* val) {
    d->getNoticeWindow()->write(tr("sending>> ") + tr( (*val).toStyledString().c_str() ));
    d->getNoticeWindow()->writeValue(0, val);
  }

  bool Interface::isConnected(){
    return connected;
  }

  std::string Interface::getServer(){
    return server_address;
  }

  void Interface::setConnected( bool con_stat ){
    connected = con_stat;
  }

  int Interface::getOutboundQueueLength() {
    return proto->getOutboundQueueLength();
  }

  int Interface::getReceivedQueueLength() {
    return proto->getReceivedQueueLength();
  }


} // end namespace Grids
