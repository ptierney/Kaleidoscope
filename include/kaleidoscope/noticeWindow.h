
#include <QWidget>

namespace Kaleidoscope {

	class NoticeWindow : public QWidget {
		Q_OBJECT
	
		public:
		// The parent will almost always be the Kaleidoscope::Device
		NoticeWindow(QWidget* parent);

		void addNotice( int priority, const QString & message);

	};
} // end namespace Kaleidoscope
