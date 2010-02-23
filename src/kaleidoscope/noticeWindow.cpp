
#include <kaleidoscope/noticeWindow.h>
#include <grids/define.h>
#include <grids/event.h>

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

        /* The process of translating a json into a styled string is
           processor intersive, so filter the low priority messages out first. */
        void NoticeWindow::writeValue(int priority, Grids::Value* val) {
            if(priority >= current_priority)
                addNotice( priority, tr( (*val).toStyledString().c_str() ) );
        }

        void NoticeWindow::writeEvent(int priority, Grids::Event* evt) {
            if(priority >= current_priority)
                addNotice( priority, tr( evt->getStyledString().c_str() ) );
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

