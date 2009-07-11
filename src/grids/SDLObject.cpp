

#include <grids/SDLObject.h>


namespace Grids {

	SDLObject::SDLObject(){
		object_mutex = SDL_CreateMutex();
	}

	SDLObject::~SDLObject(){
		SDL_DestroyMutex(object_mutex);
	}

	void SDLObject::lock(){	
		SDL_LockMutex( object_mutex );
	}

	void SDLObject::unlock(){
		SDL_UnlockMutex( object_mutex );
	}

} // end namespace Grids
