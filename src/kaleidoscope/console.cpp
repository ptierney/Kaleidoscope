
#include <kaleidoscope/console.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>


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
		else if(lower == tr("create room") ){
			createRoom();
		} 
		else if(lower == tr("create object") ){
			createObject();
		}
		else if(lower == tr("disconnect") || 
			   lower == tr("disconnect from server") ){
			disconnectFromServer();
		}
	}
	
	

} // end namespace Kaleidoscope
