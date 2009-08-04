
#include <kaleidoscope/kaleidoscope.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

	Kaleidoscope::Kaleidoscope( int argc, char** argv ){
		parseArgs( argc, argv );

		d = new Device();
	}
	
	Kaleidoscope::~Kaleidoscope(){
		delete d;
	}
	
	void Kaleidoscope::run(){
		while( d->getRunning() ){
			d->run();
		}
	}
	
	void Kaleidoscope::parseArgs( int argc, char** argv ){

	}

} // end namespace Kaleidoscope
