
#include <kaleidoscope/noticeWindow.h>
#include <QWidget>


namespace Kaleidoscope {

	NoticeWindow::NoticeWindow(QWidget* parent) 
		: QListWidget(parent) {
		
		/* Display everything at the moment */
		current_priority = 0;
	}

	void NoticeWindow::setPriority( int new_priority) {
		current_priority = new_priority;
	}

	void NoticeWindow::write(const QString & message) {
		addNotice(message);
	}

	void NoticeWindow::write(int priority, const QString & message) {
		addNotice(priority, message);
	}


	void NoticeWindow::addNotice(const QString & message ){
		addNotice( DEFAULT_PRIORITY, message);
	}

	void NoticeWindow::addNotice( int priority, const QString & message) {
		// Add something here
		if( priority >= current_priority){
			addItem(message);
		}
	}

} // end namespace Kaleidoscope

