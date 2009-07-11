

#include <grids/utility.h>

namespace Grids{

	Utility::Utility(){

	}	

	std::string Utility::getNewUUID(){
		uuid_obj.make( UUID_MAKE_V4 );

		return uuid_obj.string();
	}
	
	bool Utility::checkUUIDValidity( std::string in_id ){
		std::string temp_string;
		
		// Remove dashes
		for( int i = 0; i < in_id.length(); i++ ){
			if( in_id[i] == '-' )
				continue;
			
			temp_string.push_back( in_id[i] );
		}
		
		// Check required length
		if( temp_string.length() != 32 )
			return false;

		char temp_char;

		for( int i = 0; i < temp_string.length(); i++ ){
		
			temp_char = temp_string[i];
			if( temp_char >= '0' && temp_char <= '9' )
				continue;

			if( temp_char >= 'A' && temp_char <= 'F' )
				continue;

			if( temp_char >= 'a' && temp_char <= 'f' )
				continue;

			return false;
		}
		
		return true;
	}


} // end namespace Grids

