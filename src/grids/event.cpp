
#include <string>
#include <vector>
#include <iostream>

#include <grids/event.h>
#include <grids/define.h>

namespace Grids
{
	Event::Event( ) : event_type( GRIDS_NULL_EVENT ){
	}

	Event::Event( std::string in_event ) : event_type( in_event ) {
		//event_type = in_event;
	}

  Event::Event( std::string in_event, const Value& value )	{
		event_type = in_event;
		args = value;
	}

	Event::Event( Event& copy_evt ){
		event_type = copy_evt.getEventType();
		args = Value( copy_evt.getArgs() );
	}


	void Event::setEvent(std::string in_event)	{
		event_type = in_event;
	}

  void Event::setArgs( const Value& value)	{
    args = Value(value);
	}

  const Grids::Value& Event::getArgs()	{
		return args;
	}

  Grids::Value* Event::getArgsPtr( )	{
		return &args;
	}

  const Grids::Value& Event::getAttr(){
    return args["attr"];
  }

  Grids::Value* Event::getAttrPointer(){
    return &(args["attr"]);
  }

  std::string Event::getAttrType(){
    return args["attr"]["type"].asString();
  }

	std::string Event::getEventType()	{
		return event_type;
	}

  std::string Event::getStyledString() {
    return args.toStyledString();
  }
	
	GridsID Event::getID() {
		return args[ "id" ].asString();
	}
	
	bool Event::hasPosition(){
		return !( args[ "pos" ].empty() );
	}
	
	bool Event::hasRotation() {
		return !( args[ "rot" ].empty() );
	}

	bool Event::hasScale() {
		return !( args[ "scl" ].empty() );
	}
	
	bool Event::hasAttr() {
		return !( args[ "attr" ].empty() );
	}
	
	Vec3D Event::getPosition() {
		return Vec3D( args[ "pos" ][ 0u ].asDouble(),
                  args[ "pos" ][ 1u ].asDouble(),
                  args[ "pos" ][ 2u ].asDouble() );
	}

	Vec3D Event::getRotation() {
		return Vec3D( args[ "rot" ][ 0u ].asDouble(),
                  args[ "rot" ][ 1u ].asDouble(),
                  args[ "rot" ][ 2u ].asDouble() );
	}

	Vec3D Event::getScale() {
		return Vec3D( args[ "scl" ][ 0u ].asDouble(),
                  args[ "scl" ][ 1u ].asDouble(),
                  args[ "scl" ][ 2u ].asDouble() );
	}



}

