

#pragma once

#include <kaleidoscope/define.h>
#include <vector>
#include <QMainWindow>

namespace Grids{
	class ObjectController;
	class Interface;
	class Utility;
}

namespace Kaleidoscope {
	class Room;
	class OSWindow;
	class EventController;
	class Camera;
	class Settings;

	class Device {
	public:
		Device(QMainWindow*);
		~Device();
		
		Grids::ObjectController* getObjectController();
		Grids::Interface* getInterface();
		Grids::Utility* getGridsUtility();
		OSWindow* getOSWindow();		
		EventController* getEventController();
		Settings* getSettings();

		bool getRunning();
		void setRunning( bool );
		GridsID getMyID();
		GridsID getMyRoom();
		void setMyID(GridsID);
		void setMyRoom(GridsID);
		int getTicks();

		QMainWindow* main_window;
		Camera* main_camera;
	private:
		Grids::ObjectController* object_controller;
		Grids::Interface* interface;
		Grids::Utility* g_utility;

		EventController* event_controller;
		Settings* settings;
		Console* console;
		NoticeWindow* noticeWindow;
		NoticeWindow* errorWindow;

		void createObjects( unsigned int, unsigned int );
		void init( unsigned int, unsigned int );
		void loadRoom();
		// The three main starting boxes
		void createMainCamera();
		void createConsole();
		void createNoticeWindow();
		void createErrorWindow();
		
		QTime time;	
		GridsID my_id;		
		GridsID my_room;

		bool running;
		QMutex running_mutex;
		QMutex my_id_mutex;
		QMutex my_room_mutex;
	};

} // end namespace Kaleidoscope
