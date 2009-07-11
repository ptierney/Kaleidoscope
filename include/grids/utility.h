

#pragma once

#include <ossp/uuid++.hh>
#include <string>

namespace Grids{

	class Utility {

	public:
		Utility();
		
		std::string getNewUUID();
		
		bool checkUUIDValidity( std::string );
	
	private:
		uuid uuid_obj;
	};

} // end namespace Grids
