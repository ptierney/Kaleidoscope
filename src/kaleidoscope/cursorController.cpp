
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/device.h>

#include <QWidget>
#include <QMouseEvent>

namespace Kaleidoscope
{
	CursorController::CursorController(Device* dvc, QWidget* wid) {
		d = dvc;
		widget = wid;
	}	

	void CursorController::setPosition( float xScale, float yScale ) {
		int wid_wid = widget->width();
		int wid_hei = widget->height();
		
		QPoint wid_center_point = QPoint( (int)(xScale*(widget->width())), 
								(int)(yScale*(widget->height())));

		QPoint global_pnt = widget->mapToGlobal( wid_center_point);

		int new_x = global_pnt.x();
		int new_y = global_pnt.y();

		widget->cursor().setPos(new_x, new_y);
		//widget->cursor().setPos(100, 100);
	}
	
        void CursorController::setToCenter() {
		setPosition( 0.5f, 0.5f );
	}
	
	int CursorController::getMouseX() {
                return widget->mapFromGlobal(QPoint(widget->cursor().pos().x(),
                                                                         widget->cursor().pos().y())).x();
	}

	int CursorController::getMouseY() {
                return widget->mapFromGlobal(QPoint(widget->cursor().pos().x(),
                                                                         widget->cursor().pos().y())).y();
	}
	
	Vec2D CursorController::getRelativePosition() {
		return Vec2D((float)getMouseX() / (float) widget->width(),
				   (float)getMouseY() / (float) widget->height());
	}
	
	Vec2D CursorController::getRelativePosition(QMouseEvent* event){
		return Vec2D((float)event->x() / (float)widget->width(),
				   (float)event->y() / (float)widget->height());
	}
	
	void CursorController::hideCursor() {
                d->app->setOverrideCursor(QCursor(Qt::BlankCursor));
	}
	
	void CursorController::showCursor() {
                d->app->restoreOverrideCursor();
	}

} // end namespace Kaleidoscope
