
#include <kaleidoscope/utility.h>
#include <kaleidoscope/define.h>
#include <string> 
#include <iostream>

namespace Kaleidoscope {

	// The higher the debug_level, the less often it will be shown

	void Utility::puts( int debug_level, std::string str1 ){
		if( debug_level < KALEIDOSCOPE_DEBUG_LEVEL )
			std::cout << str1 << std::endl;
	}

	void Utility::puts( int debug_level, std::string str1, std::string str2 ){
		if( debug_level < KALEIDOSCOPE_DEBUG_LEVEL )
			std::cout << str1 << "   " << str2 << std::endl;
	}

} // end namespace Kaleidoscope
