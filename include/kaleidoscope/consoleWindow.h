
#pragma once

#include <QLineEdit>

namespace Kaleidoscope {

	class ConsoleWindow : public QLineEdit {
		Q_OBJECT
	
		public:
		// The parent will almost always be the Kaleidoscope::Device
		ConsoleWindow(QWidget* parent);
		    
	private:

	};
} // end namespace Kaleidoscope
