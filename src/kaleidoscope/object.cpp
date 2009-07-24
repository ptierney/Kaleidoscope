
#include <kaleidoscope/object.h>
#include <kaleidoscope/define.h>

#include <grids/object.h>

namespace Kaleidoscope {

	Object::Object( Device* d, Grids::Value* val ) : Grids::Object( d, val ) {
		visible_mutex = SDL_CreateMutex();
		selected_mutex = SDL_CreateMutex();		

		show();
	}
	
	void Object::drawAll(Device* d){
          if( getVisibility() ){
			std::vector< Grids::Object* > temp_children = getChildren();

               for( int i = 0; i < temp_children.size(); i++ ){
                    temp_children[i]->drawAll( d );
               }
               draw( d ); // draw yourself
          }
	}

	bool Object::detectSelection(){

	}

	////////////////////////////
	// Accessor functions

	void Object::show(){
		SDL_LockMutex( visible_mutex );
		visible = 1;
		SDL_UnlockMutex( visible_mutex );
	}

	void Object::hide(){
		SDL_LockMutex( visible_mutex );
		visible = 0;
		SDL_UnlockMutex( visible_mutex );
	}

	bool Object::getVisibility(){
		bool temp_vis;
		
		SDL_LockMutex( visible_mutex );
		temp_vis = visible;
		SDL_UnlockMutex( visible_mutex );
		
		return temp_vis;
	}

	void Object::select(){
		SDL_LockMutex( selected_mutex );
		selected = 1;
		SDL_UnlockMutex( selected_mutex );
	}	

	void Object::deselect(){
		SDL_LockMutex( selected_mutex );
		selected = 0;
		SDL_UnlockMutex( selected_mutex );
	}

} // end namespace Kaleidoscope
