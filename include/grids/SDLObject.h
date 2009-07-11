
#pragma once


#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>


namespace Grids{

	class SDLObject{
	protected:
		SDLObject();
		~SDLObject();

		void lock();
		void unlock();

	private:
		SDL_mutex* object_mutex;
	};

} // end namespace Grids
