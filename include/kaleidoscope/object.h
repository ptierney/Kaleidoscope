
#pragma once

#include <grids/object.h> 
#include <SDL/SDL_thread.h>

namespace Kaleidoscope {
	class Device;

	class Object : public Grids::Object {

	public:
		Object( Device*, Grids::Value* );
	
		void show();
		void hide();
		 
		virtual bool detectSelection();
		virtual void select();
		virtual void deselect();
		
	protected:
		virtual void draw( Device* ) = 0;
		virtual void drawAll(Device*);		

	private:
		bool visible;
		bool selected;
	
		SDL_mutex* visible_mutex;
		SDL_mutex* selected_mutex;

	};

} // end namespace Kaleidoscope
