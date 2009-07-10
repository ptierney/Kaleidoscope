
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

	Event::Event( std::string in_event, Value value )	{
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

	void Event::setArgs( Value value)	{
		args = value;
	}

	Grids::Value Event::getArgs()	{
		return args;
	}

	Grids::Value * Event::getArgsPtr( )	{
		return &args;
	}

	std::string Event::getEventType()	{
		return event_type;
	}



}

