

#pragma once

#include <QWidget>
#include <iostream>

namespace Kaleidoscope {
	class Device;	

	class Kaleidoscope : public QWidget{
	Q_OBJECT
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
