

#pragma once

#include <iostream>

namespace Kaleidoscope {
	class Device;	

	class Kaleidoscope{
		
	public:
		Kaleidoscope( int argc, char** argv );		
		~Kaleidoscope();
	
		void run();
		
		void parseArgs( int, char** );

	private:
		Device* d;

		int w;
		int h; 
	};

} // end namespace Kaleidoscope

namespace Kal = Kaleidoscope;
