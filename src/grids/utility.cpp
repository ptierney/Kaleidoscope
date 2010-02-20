
#include <grids/utility.h>
#include <stdlib.h>
#include <time.h>
#include <string>

namespace Grids{

	Utility::Utility(){
	  srand( time( NULL ) );
	}	

	std::string Utility::getNewUUID(){
	  return cMakeUUID();
	}
	
  std::string Utility::cMakeUUID(){
	std::string temp_uuid;
	unsigned int ran_num;
	for( int i = 0; i < 36; i++ ){
	  if( i == 8 || i == 13 || i == 18 || i == 23){
		temp_uuid.push_back( '-' );
		continue;
	  }
	
	  ran_num = rand() % 16;
	  temp_uuid.push_back( hexToChar( ran_num ) );		
	}
	
	return temp_uuid;
  }

  char Utility::hexToChar( unsigned int num ){	
	if( num < 10 ){
	  return '0' + (char)num;
	} else if ( num < 16 ){
	  return 'A' + (char)(num - 10);
	} else {
	  return '0';
	}
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

