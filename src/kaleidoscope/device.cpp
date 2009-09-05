
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
	
	Device::Device(QMainWindow* m_win){
		main_window = m_win;
		main_camera = 0;
		time.start();

		createObjects(m_win->size.width(), m_win->size.height());
		init(m_win->size.width(), m_win->size.height());
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
		// Window should be one of the last objects to be destroyed, as it calls SDL_Quit()
		delete window;
		delete event_controller;
		delete settings;
	}
		
	Grids::ObjectController* Device::getObjectController(){ return object_controller; }
	Grids::Interface* Device::getInterface(){ return interface; }	
	Grids::Utility* Device::getGridsUtility(){ return g_utility; }
	OSWindow* Device::getOSWindow() { return window; }	
	Settings* Device::getSettings() { return settings; }	
	EventController* Device::getEventController(){ return event_controller; }

	void Device::createMainCamera() {

	}

	void Device::createConsole() {

	}

	void Device::createNoticeWindow() {

	}

	void Device::createErrorWindow() {

	}

	void Device::loadRoom(){		
		Utility::puts( 1, "Your room:  ", getInterface()->createMyRoom( 20 ) );
	}

	// The three main starting boxes
	void Device::createMainCamera() {
	}

	void Device::createConsole() {
		QDockWidget *dock = new QDockWidget(tr("Console"), main_window);
		console = new Console(dock);
		dock->setWidget(console);
		main_window->addDockWidget(Qt::BottomDockWidgetArea, dock);
	}

	void Device::createNoticeWindow() {
		QDockWidget *dock = new QDockWidget(tr("Notices"), main_window);
		noticeWindow = new NoticeWindow(dock);
		dock->setWidget(noticeWindow);
		main_window->addDockWidget(Qt::RightDockWidgetArea, dock);
	}

	void Device::createErrorWindow() {
		QDockWidget *dock = new QDockWidget(tr("Errors"), main_window);
		errorWindow = new NoticeWindow(dock);
		dock->setWidget(errorWindow);
		main_window->addDockWidget(Qt::RightDockWidgetArea, dock);
	}
	
	int Device::getTicks() {
		return time.elapsed();
	}
		
	/////////////////////////////////////
	// Accessor Functions
	
	bool Device::getRunning() {
		QMutexLocker(&running_mutex);
		return running;
	}
	
	void Device::setRunning( bool new_run ){
		QMutexLocker(&running_mutex);
		running = new_run;
	}

	GridsID Device::getMyID(){
		QMutexLocker(&my_id_mutex);
		return my_id;
	}
	
	void Device::setMyID( GridsID temp_id ){
		QMutexLocker(&my_id_mutex);
		my_id = temp_id;
	}
	
	GridsID Device::getMyRoom() {
		QMutexLocker(&my_room_mutex);
		return my_room;
	}
} // end namespace Kaleidoscope
