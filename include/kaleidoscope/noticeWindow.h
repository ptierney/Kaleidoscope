
#pragma once

#include <QListWidget>

#include <grids/define.h>

namespace Grids {
    class Event;
}

namespace Kaleidoscope {

	class NoticeWindow : public QListWidget {
		Q_OBJECT
	
		public:
		// The parent will almost always be the Kaleidoscope::Device
		NoticeWindow(QWidget* parent);

		/* Display a notice */
		void write( const QString &);
		void write( int priority, const QString & message);
                void writeValue(int priority, Grids::Value*);
                void writeEvent(int priority, Grids::Event*);

		void addNotice( const QString & message);
		void addNotice( int priority, const QString & message);

		/* Set the limit of what messages are displayed */
		void setPriority( int );

	private:
		// Priority ranges from 0 (Lowest), to 10(Highest), and to 11
		static const int DEFAULT_PRIORITY = 5; 

		int current_priority;
	};

} // end namespace Kaleidoscope
