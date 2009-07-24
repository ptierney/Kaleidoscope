

#include <kaleidoscope/kaleidoscope.h>
#include <iostream>

int main( int argc, char **argv ) {

	Kal::Kaleidoscope* mk = new Kal::Kaleidoscope( argc, argv );
		
	mk->run();

	delete mk;
		
	std::cout << "Exited Cleanly." << std::endl;
	
	return 0;
}
