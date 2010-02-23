
#include <kaleidoscope/console.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/genericNodeItem.h>
#include <grids/interface.h>


namespace Kaleidoscope {

	Console::Console(Device* d, QWidget* window_parent){
		this->d = d;
	
		console_window = new ConsoleWindow(window_parent);
		
		// link returnPressed with newInput

		QObject::connect(console_window, SIGNAL( returnPressed() ),
					  this, SLOT(newInput() ) );

		QObject::connect(this, SIGNAL( clearLine() ),
					  console_window, SLOT( clear() ));
	}
	
	ConsoleWindow* Console::getConsoleWindow() {
		return console_window;
	}
	
	void Console::newInput() {		
		QString input_text = console_window->text();
		
		d->getNoticeWindow()->write(tr("> ") + input_text);		
		clearLine();

		parseInput(input_text);
	}
	
	void Console::parseInput(QString input){
		QString lower = input.toLower();

		if(lower == tr("connect") ||
		   lower == tr("conect to server") ){
			connectToServer();
		}
		else if(lower == tr("exit") ||
			   lower == tr("quit") ){
                        d->quit();
		}
		else if(lower == tr("create room") ){
                    d->getInterface()->createMyRoom();
                        //createRoom();
		} 
                else if(lower == tr("join room")) {
                    d->getInterface()->requestAllRooms();
                }
		else if(lower == tr("create object") ){
			createObject();
		}
                else if(lower == tr("generic node")) {
                    GenericNodeItem::requestCreate(d, Vec3D(1.0, 1.0, 1.0), "Generic Node");
                }
		else if(lower == tr("disconnect") || 
			   lower == tr("disconnect from server") ){
			disconnectFromServer();
		}

	}	

} // end namespace Kaleidoscope
