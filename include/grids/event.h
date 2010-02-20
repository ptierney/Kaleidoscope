

#pragma once
 
#include <string>
#include <map>
#include <vector>
#include <queue>

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

                std::string getStyledString();
		
		bool hasPosition();
		bool hasRotation();
		bool hasScale();
		bool hasAttr();
		GridsID getID();		
		Vec3D getPosition();
		Vec3D getRotation();
		Vec3D getScale();

	private:
		std::string event_type;
		Grids::Value args;
	};

        typedef std::queue<Event*> EventQueue;
}


