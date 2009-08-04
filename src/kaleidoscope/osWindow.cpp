
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/renderer.h>
#include <kaleidoscope/device.h>
#include <grids/interface.h>
#include <grids/utility.h>

#include <iostream>
#include <assert.h>

#include <SDL/SDL_ttf.h>

namespace Kaleidoscope {

	OSWindow::OSWindow( Device* d, unsigned int width, unsigned int height ){
		this->width = width;
		this->height = height;
		this->d = d;
		current_camera = -1;
			
		cursor = new CursorController( d );
		
		createMutexes();
		initSDL();
	}
	
	void OSWindow::init(Device* d){
		requestCreateCamera(d);
	}
	
	OSWindow::~OSWindow(){
		delete cursor;
		//renderer is deleted automatically 
		//cameras are too		

		TTF_Quit();
		SDL_Quit();
	}
	
	void OSWindow::createMutexes( ){
		width_mutex = SDL_CreateMutex();
		height_mutex = SDL_CreateMutex();
	}
	
	unsigned int OSWindow::getWidth() { unsigned int temp_w;  SDL_LockMutex( width_mutex ); temp_w = width; SDL_UnlockMutex( width_mutex ); return temp_w; }
	unsigned int OSWindow::getHeight() { unsigned int temp_h;  SDL_LockMutex( height_mutex ); temp_h = height; SDL_UnlockMutex( height_mutex ); return temp_h; }
	void OSWindow::setWidth( unsigned int new_width ) { SDL_LockMutex( width_mutex ); width = new_width; SDL_UnlockMutex( width_mutex ); }
	void OSWindow::setHeight( unsigned int new_height ) { SDL_LockMutex( height_mutex ); height = new_height; SDL_UnlockMutex( height_mutex ); }
	
	CursorController* OSWindow::getCursorController() {
		return cursor;
	}

	Camera* OSWindow::getCamera() { 
		if( current_camera < 0 )
			return NULL;
		else
			return cameras[ current_camera ]; 
	}
	
	Renderer* OSWindow::getRenderer() { return renderer; }

	void OSWindow::initSDL(){
		if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {
			std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
			assert( false );
		}

		// Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
		// Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
		// 5-5-5 RGB for 16-bit screens
		// Request a 16-bit depth buffer (without this, there is no depth buffer)
		int value = 16;
		SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);

		// Request double-buffered OpenGL
		// The fact that windows are double-buffered on Mac OS X has no effect
		// on OpenGL double buffering.
		value = 1;
		SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, value);

		// SDL_Surface * temp_surface = SDL_LoadBMP( "change_room.bmp" );
		// SDL_WM_SetIcon( temp_surface, NULL );
		SDL_WM_SetCaption( "Grids", "Kaleidoscope" );

		//The surface returned is freed by SDL_Quit() and should not be freed by the caller. 
		// 0 automatically selects the best availible BPP
		// SDL_HWSURFACE : use hardware rendering
		gl_screen = SDL_SetVideoMode( width, height, 0, SDL_OPENGL | SDL_HWSURFACE );

		if ( gl_screen  == NULL ) {
			std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
			assert( false );
		}
		
		TTF_Init();
	}
	
	void OSWindow::doMovement(Device* d) {
		if( current_camera >= 0 )
			cameras[ current_camera ]->doMovement( d );
	}
	
	void OSWindow::renderAll() {		
		renderer->renderAll( d ); 
				
		SDL_GL_SwapBuffers();
	}

	void OSWindow::requestCreateCamera(Device* d){
		Vec3D start_pos = Vec3D( 10.0f, 30.0f, 35.0f );
		Vec3D start_rot = Vec3D( 1.0f, -1.0f, 0.0f );
		Vec3D start_scl = Vec3D( 1.0f, 1.0f, 1.0f );
		
		Grids::Value* cam_val = new Grids::Value();
		
		(*cam_val)[ "type" ] = "Camera";
		(*cam_val)[ "parent" ] = d->getMyID();
		(*cam_val)[ "camera_type" ] = FPS;
		(*cam_val)[ "rotate_speed" ] = 30.0f;
		(*cam_val)[ "translate_speed" ] = 0.01f;
		(*cam_val)[ "target" ][ 0u ] = -1.0f;
		(*cam_val)[ "target" ][ 1u ] = -1.0f;
		(*cam_val)[ "target" ][ 2u ] = -1.0f;
		(*cam_val)[ "up" ][ 0u ] = 0.0f;
		(*cam_val)[ "up" ][ 1u ] = 1.0f;
		(*cam_val)[ "up" ][ 2u ] = 0.0f;
		
		d->getInterface()->requestCreateObject( cam_val, start_pos, start_rot, start_scl );

		delete cam_val;
	}
	
	void OSWindow::registerCamera(Camera* cam){
		if( current_camera < 0 )
			current_camera = 0;
		else
			current_camera = cameras.size();
		
		cameras.push_back( cam );
		cam->resizeScene(d, getWidth(), getHeight() );
	}

	void OSWindow::createRenderer(Device* d){
		Grids::Value* ren = new Grids::Value();
		
		(*ren)[ "id" ] = d->getGridsUtility()->getNewUUID();
		(*ren)[ "req" ][ "pos" ][ 0u ] = 0.0f;
		(*ren)[ "req" ][ "pos" ][ 1u ] = 0.0f;
		(*ren)[ "req" ][ "pos" ][ 2u ] = 0.0f;
		(*ren)[ "req" ][ "rot" ][ 0u ] = 0.0f;
		(*ren)[ "req" ][ "rot" ][ 1u ] = 0.0f;
		(*ren)[ "req" ][ "rot" ][ 2u ] = 0.0f;
		(*ren)[ "req" ][ "scl" ][ 0u ] = 1.0f;
		(*ren)[ "req" ][ "scl" ][ 1u ] = 1.0f;
		(*ren)[ "req" ][ "scl" ][ 2u ] = 1.0f;
		
		renderer = new Renderer( d, ren );
		
		delete ren;
	}


} // end namespace Kaleidoscope
