


#include <QWidget>

namespace Kaleidoscope {

	class Console : public QWidget {
		Q_OBJECT
	
		public:
		// The parent will almost always be the Kaleidoscope::Device
		Console(QWidget* parent);

	};
} // end namespace Kaleidoscope
