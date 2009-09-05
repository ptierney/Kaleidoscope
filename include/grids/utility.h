
#pragma once

#include <string>

namespace Grids{

	class Utility {

	public:
		Utility();
		
		std::string getNewUUID();
		
		bool checkUUIDValidity( std::string );
	
	private:
		std::string cMakeUUID();
		char hexToChar( unsigned int );

	};

} // end namespace Grids
