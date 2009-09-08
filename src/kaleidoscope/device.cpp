
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
	Device::Device(QMainWindow* m_win) {
		main_window = m_win;
		main_camera = 0;
		time.start();
		
		// NoticeWindow & ErrorWindow must be created before any Grids::Objects,
		// as all Grids::Objects require them to be created
		createNoticeWindow();
		createErrorWindow();

		createObjects();
		init();
	}

	void Device::createObjects(){
		// Settings stores / loads the user data
		settings = new Settings();

		// Derived from QWidget, handles misc. key presses
		event_controller = new Kal::EventController(this, main_window);

		object_controller = new Grids::ObjectController(this, main_window);
		// Creates the protocol, connects to the server
		interface = new Grids::Interface(this, main_window);
		g_utility = new Grids::Utility();
	}
		
	void Device::init( unsigned int sw, unsigned int sh ) {
		running = 1;
		setMyID( "7A293FB2-70C9-11DE-B84C-43FC4C661FD7" );		

		// Requests a new camera from the server
		// Creates and registers a new renderer
		requestCreateCamera();
		loadRoom();
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
	Settings* Device::getSettings() { return settings; }	
	EventController* Device::getEventController(){ return event_controller; }
	NoticeWindow* getNoticeWindow() { return noticeWindow; }
	NoticeWindow* getErrorWindow() { return errorWindow; }

	void Device::loadRoom(){		
		Utility::puts( 1, "Your room:  ", getInterface()->createMyRoom( 20 ) );
	}

	// The four main starting boxes: camera, notices, errors, console
	// 

	// Create a new camera, and if it belongs to us, register it as the main 
	// camera.
	void Device::registerCamera(Grids::Event* event) {
		QDockWidget *dock = new QDockWidget(tr("Camera"), main_window);
		Camera* temp_camera = new Camera(this, event->getArgsPtr(), dock);

		if((*(evt->getArgsPtr()))[ "req" ][ "attr" ][ "parent" ].asString() == d->getMyID()) {
			main_camera = temp_camera;
		}

		dock->setWidget(temp_camera);
		main_window->addDockWidget(Qt::LeftDockWidgetArea, dock);
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

	void Device::requestCreateCamera(){
		Vec3D start_pos = Vec3D( 10.0f, 30.0f, 35.0f );
		Vec3D start_rot = Vec3D( 1.0f, -1.0f, 0.0f );
		Vec3D start_scl = Vec3D( 1.0f, 1.0f, 1.0f );
		
		Grids::Value* cam_val = new Grids::Value();
		
		(*cam_val)[ "type" ] = "Camera";
		(*cam_val)[ "parent" ] = getMyID();
		(*cam_val)[ "camera_type" ] = FPS;
		(*cam_val)[ "rotate_speed" ] = 30.0f;
		(*cam_val)[ "translate_speed" ] = 0.01f;
		(*cam_val)[ "target" ][ 0u ] = -1.0f;
		(*cam_val)[ "target" ][ 1u ] = -1.0f;
		(*cam_val)[ "target" ][ 2u ] = -1.0f;
		(*cam_val)[ "up" ][ 0u ] = 0.0f;
		(*cam_val)[ "up" ][ 1u ] = 1.0f;
		(*cam_val)[ "up" ][ 2u ] = 0.0f;
		
		interface->requestCreateObject( cam_val, start_pos, start_rot, start_scl );

		delete cam_val;
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
