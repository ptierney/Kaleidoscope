

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
    Event( std::string, const Grids::Value& );
		Event( Event& );

		void setEvent( std::string );
    void setArgs( const Grids::Value& );

    const Grids::Value& getArgs();
    Grids::Value* getArgsPtr();
    const Grids::Value& getAttr();
    Grids::Value* getAttrPointer();
		std::string getEventType();
    std::string getAttrType();

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


