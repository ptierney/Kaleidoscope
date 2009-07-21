

#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>

#include <grids/objectController.h>
#include <grids/interface.h>
#include <grids/utility.h>

namespace Kaleidoscope {

	Device::Device(){
		init( 640, 480 );
	}
	
	Device::Device( unsigned int sw, unsigned int sh ){
		init( sw, sh );
	}
		
	void Device::init( unsigned int sw, unsigned int sh ){
		object_controller = new Grids::ObjectController();
		interface = new Grids::Interface( this );
		g_utility = new Grids::Utility();
	}

	Device::~Device(){
		delete object_controller;
		delete interface;
		delete g_utility;
	}
		
	Grids::ObjectController* Device::getObjectController(){ return object_controller; }
	Grids::Interface* Device::getInterface(){ return interface; }	
	Grids::Utility* Device::getGridsUtility(){ return g_utility; };
	

} // end namespace Kaleidoscope
