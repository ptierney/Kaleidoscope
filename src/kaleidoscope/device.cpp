

#include <kaleidoscope/device.h>
#include <grids/objectController.h>

namespace Kaleidoscope {

	Device::Device(){
		init( 640, 480 );
	}
	
	Device::Device( unsigned int sw, unsigned int sh ){
		init( sw, sh );
	}
		
	void Device::init( unsigned int sw, unsigned int sh ){
		object_controller = new Grids::ObjectController();
	}

	Device::~Device(){

	}
		
	Grids::ObjectController* Device::getObjectController(){
		return object_controller;
	}


} // end namespace Kaleidoscope
