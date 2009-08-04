
#pragma once

#include <SDL/SDL.h>
#include <vector>

namespace Kaleidoscope {
	class CursorController;
	class Camera;
	class Renderer;
	class Device;

	class OSWindow {
	public:
		OSWindow( Device*, unsigned int width, unsigned int height);
		~OSWindow();
		void init(Device*);

		// Calls the camera and does the appropriate movement
		void doMovement(Device*);
			
		// Calls the renderer
		void renderAll();
		
		void createRenderer(Device* );
		void requestCreateCamera(Device* d);
		void registerCamera( Camera* );
		Camera* getCamera();
		Renderer* getRenderer();
		CursorController* getCursorController();

		unsigned int getWidth();
		unsigned int getHeight();
		void setWidth(unsigned int);
		void setHeight(unsigned int);

	private:
		void initSDL();
		void createMutexes();
		
		unsigned int width;
		unsigned int height;
		
		CursorController* cursor;
		int current_camera;
		std::vector< Camera* > cameras;
		Renderer* renderer;

		SDL_Surface* gl_screen;
		
		SDL_mutex* width_mutex;
		SDL_mutex* height_mutex;
		
		Device* d;
	};

} // end namespace Kaleidoscope
