
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
		widget->cursor().setPos(widget->mapToGlobal(QPoint((int)(xScale*widget->width()),
												 (int)(yScale*widget->height()))));
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
		d->app->setOverrideCursor(QCursor(Qt::ArrowCursor));
	}

} // end namespace Kaleidoscope
