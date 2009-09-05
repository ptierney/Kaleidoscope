
#pragma once

#include <grids/object.h> 
#include <QMutex>

namespace Kaleidoscope {
	class Device;

	class Object : public Grids::Object {

	public:
		Object(Device*, Grids::Value*);

		virtual void drawAll(Device*);		
	
		void show();
		void hide();
		bool getVisibility();		
		 
		virtual bool detectSelection();
		virtual void select();
		virtual void deselect();

	protected:
		virtual void draw(Device*) = 0;

	private:
		bool visible;
		bool selected;
	
		QMutex visible_mutex;
		QMutex selected_mutex;
	};

} // end namespace Kaleidoscope
