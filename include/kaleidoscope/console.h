
#pragma once

#include <QWidget>

namespace Kaleidoscope {

	class Device;
	class ConsoleWindow;

	class Console : public QObject {
	Q_OBJECT	

	public:
	Console(Device* d, QWidget* window_parent) ;

	ConsoleWindow* getConsoleWindow();

	public slots:
	// This is called by the Console Window when the enter or return key is pressed
	void newInput();
	
	signals:
	void clearLine();
	void connectToServer();
	void disconnectFromServer();
	void createRoom();
	void createObject();

	private:
		Device* d;
		ConsoleWindow* console_window;

		void parseInput(QString);

		/* TEST FUNCTIONS */
		void testCursorCenter();
	};

} // end namespace Kaleidoscope
