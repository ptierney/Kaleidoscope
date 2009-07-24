
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/settings.h>
#include <kaleidoscope/utility.h>
#include <kaleidoscope/renderer.h>

#include <grids/objectController.h>
#include <grids/interface.h>
#include <grids/utility.h>

namespace Kaleidoscope {

	Device::Device(){
		createObjects( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
		init( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );	
	}
	
	Device::Device( unsigned int sw, unsigned int sh ){
		createObjects( sw, sh );
		init( sw, sh );
		loadRoom();
	}

	// Object creation is done in two passes
	// The first pass creates the objects and stores them in the Device class
	// The second pass calls any init functions that spawn threads
	void Device::createObjects(unsigned int sw, unsigned int sh ) {
		settings = new Settings();
		// Inits SDL
		// Spawns new window
		window = new Kal::OSWindow( this, sw, sh );
		event_controller = new Kal::EventController( this );

		object_controller = new Grids::ObjectController(this);
		interface = new Grids::Interface( this );
		g_utility = new Grids::Utility();		
	}
		
	void Device::init( unsigned int sw, unsigned int sh ) {
		running = 1;
		running_mutex = SDL_CreateMutex();
		my_id_mutex = SDL_CreateMutex();
		my_room_mutex = SDL_CreateMutex();
		setMyID( "7A293FB2-70C9-11DE-B84C-43FC4C661FD7" );		

		// Spawns new Protocol and network listening thread
		// Sets up SDL_net
		// This call will hang if there is no connection to the internet
		interface->init();

		// Rooms become children of the renderer, so this mush be created first
		window->createRenderer(this);		
		window->getRenderer()->prepareWindow(this);		

		loadRoom();		
		
		// Requests a new camera from the server
		// Creates and registers a new renderer
		window->init(this);
	}

	Device::~Device() {
		delete object_controller;
		delete interface;
		delete g_utility;
		delete window;
		delete event_controller;
		delete settings;
		
		SDL_DestroyMutex( running_mutex );
	}
		
	Grids::ObjectController* Device::getObjectController(){ return object_controller; }
	Grids::Interface* Device::getInterface(){ return interface; }	
	Grids::Utility* Device::getGridsUtility(){ return g_utility; }
	OSWindow* Device::getOSWindow() { return window; }	
	Settings* Device::getSettings() { return settings; }	

	void Device::run() {
		event_controller->checkEvents();
		window->doMovement(this);
		window->renderAll();
	}

	void Device::loadRoom(){		
		Utility::puts( 1, "Your room:  ", getInterface()->createMyRoom( 20 ) );
	}



	/////////////////////////////////////
	// Accessor Functions
	
	bool Device::getRunning() {
		bool temp_run;
		
		SDL_LockMutex( running_mutex );
		temp_run = running;
		SDL_UnlockMutex( running_mutex );

		return temp_run;
	}
	
	void Device::setRunning( bool new_run ){
		SDL_LockMutex( running_mutex );
		running = new_run;
		SDL_UnlockMutex( running_mutex );
	}

	GridsID Device::getMyID(){
		GridsID temp_id;
		
		SDL_LockMutex( my_id_mutex );
		temp_id = my_id;
		SDL_UnlockMutex( my_id_mutex );
		
		return temp_id;
	}
	
	void Device::setMyID( GridsID temp_id ){
		SDL_LockMutex( my_id_mutex );
		my_id = temp_id;
		SDL_UnlockMutex( my_id_mutex );
	}

} // end namespace Kaleidoscope
