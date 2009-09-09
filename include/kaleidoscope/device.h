
#pragma once

#include <kaleidoscope/define.h>
#include <vector>
#include <QMainWindow>
#include <QApplication>
#include <QCursor>
#include <QTime>
#include <QMutex>

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
	class NoticeWindow;
	class Console;
	class Renderer;

	class Device {
	public:
		Device(QApplication*, QMainWindow*);
		~Device();
		
		Grids::ObjectController* getObjectController();
		Grids::Interface* getInterface();
		Grids::Utility* getGridsUtility();
		EventController* getEventController();
		Settings* getSettings();
                Renderer* getRenderer();
		Renderer* renderer;

		bool getRunning();
		void setRunning( bool );
		GridsID getMyID();
		GridsID getMyRoom();
		void setMyID(GridsID);
		void setMyRoom(GridsID);
		int getTicks();
		NoticeWindow* getNoticeWindow();
		NoticeWindow* getErrowWindow();

		QMainWindow* main_window;
		// Cameras vs Renderers:
		// There is only 1 renderer of the program, because there is only one
		// "space"/"world" of the program.  However, there can be many views 
		// into this space, and each view is represented by a camera.
		// Each view requires a camera, which sets up the viewport.  The renderer
		// (the same renderer for each camera), then draws the world.
		// Cameras are grids objects, which have IDs and icons in the world, 
		// and must be requested through the Interface.
		// The renderer is the current OpenGL means of displaying the world
		// on the computer screen.
		Camera* main_camera;
		QApplication* app;

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
