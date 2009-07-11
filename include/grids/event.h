

#pragma once
 
#include <string>
#include <map>
#include <vector>

#include <grids/define.h>


namespace Grids
{
	class Event
	{
	public:
		Event( );
		Event( std::string );
		Event( std::string, Grids::Value );
		Event( Event& );

		void setEvent( std::string );
		void setArgs( Grids::Value );

		Grids::Value getArgs();
		Grids::Value *getArgsPtr();
		std::string getEventType();


	private:
		std::string event_type;
		Grids::Value args;

	};

}


