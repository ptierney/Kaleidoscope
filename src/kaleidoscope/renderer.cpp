

#include <kaleidoscope/renderer.h>
#include <kaleidoscope/osWindow.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/device.h>
#include <iostream>

namespace Kaleidoscope {

	Renderer::Renderer( Device* d, Grids::Value* val ) : Object( d, val ) {
		gl_mutex = SDL_CreateMutex();
		initVars();
	}

	void Renderer::draw(Device* d){
		/*glBegin( GL_LINES );
		glColor4f( 1.f, 1.f, 1.f, 1.f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 100.f, 100.f, 100.f );		
		glEnd();*/
	}

	void Renderer::renderAll(Device* d) {
		prepareRender( d );
		drawAll( d );
		renderGui( d );
		finishRender( d );
	}
	
	void Renderer::initVars(){
		Texture_On = false;
          Light_On = false;
          Alpha_Add = false;
          Blend_On = true;
          Filtering_On = false;
          Smooth_On = true;
	
		loadLights();
          loadTextModes();
	}

	void Renderer::loadLights(){
          Light_Ambient[0] =  0.1f;
          Light_Ambient[1] =  0.1f;
          Light_Ambient[2] =  0.1f;
          Light_Ambient[3] =  1.0f;

          Light_Diffuse[0] =  1.2f;
          Light_Diffuse[1] =  1.2f;
          Light_Diffuse[2] =  1.2f;
          Light_Diffuse[3] =  1.0f;

          Light_Position[0] = 2.0f;
          Light_Position[1] = 2.0f;
          Light_Position[2] = 0.0f;
          Light_Position[3] = 1.0f;
	}

	void Renderer::loadTextModes(){
          current_text_mode = 2;
          text_mode_string[0] = "GL_DECAL";
          text_mode_string[1] = "GL_MODULATE";
          text_mode_string[2] = "GL_BLEND";
          text_mode_string[3] = "GL_REPLACE";

          text_modes[0] = GL_DECAL;
          text_modes[1] = GL_MODULATE;
          text_modes[2] = GL_BLEND;
          text_modes[3] = GL_REPLACE;
	}

	void Renderer::setTextureOn( bool in_bool ) { lock(); Texture_On = in_bool; unlock(); }
	void Renderer::setLightOn( bool in_bool ) { lock(); Light_On = in_bool; unlock(); }
	void Renderer::setAlphaAdd( bool in_bool ) { lock(); Alpha_Add = in_bool; unlock(); }
	void Renderer::setBlendOn( bool in_bool ) { lock(); Blend_On = in_bool; unlock(); }
	void Renderer::setFilteringOn( bool in_bool ) { lock(); Filtering_On = in_bool; unlock(); }
	void Renderer::setSmoothOn( bool in_bool ) { lock(); Smooth_On = in_bool; unlock(); }

	void Renderer::prepareWindow( Device* d ){
		//buildTextures();   
		
		lockGL();

		// Color to clear color buffer to.
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

		// Depth to clear depth buffer to; type of test.
		glClearDepth(1.0);
		glDepthFunc(GL_LESS); 

		// Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
		glShadeModel(GL_SMOOTH);
	   
		glEnable(GL_BLEND);
		
		// Load up the correct perspective matrix; using a callback directly.
		//resizeScene(d, d->getOSWindow()->getWidth(), d->getOSWindow()->getHeight() );

		// Set up a light, turn it on.
		glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
		glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse); 
		glEnable (GL_LIGHT1); 
	
		// A handy trick -- have surface material mirror the color.
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		
		unlockGL();
	}


	void Renderer::prepareRender( Device* d ) {
          bool text_on;
          bool light_on;
          bool al_on;
          bool blend_on;
          bool filt_on;
          bool smooth_on;

		lock();
          text_on = Texture_On;
          light_on = Light_On;
          al_on = Alpha_Add;
          blend_on = Blend_On;
          filt_on = Filtering_On;
          smooth_on = Smooth_On;
          unlock();
			
		lockGL();

          if ( text_on )
               glEnable(GL_TEXTURE_2D);
          else
               glDisable(GL_TEXTURE_2D);

          if ( light_on )
               glEnable(GL_LIGHTING);
          else
               glDisable(GL_LIGHTING);

          if ( al_on )
               glBlendFunc(GL_SRC_ALPHA,GL_ONE);
          else
               glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

          // If we're blending, we don't want z-buffering.
          if ( blend_on )
               glDisable(GL_DEPTH_TEST);
          else
               glEnable(GL_DEPTH_TEST);

          if ( filt_on ) {
               glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                               GL_LINEAR_MIPMAP_LINEAR);
               glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          } else {
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                               GL_NEAREST_MIPMAP_NEAREST);
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
          }

          if ( smooth_on ){
			glHint( GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
		}

          // Need to manipulate the ModelView matrix to move our model around.
          glMatrixMode(GL_MODELVIEW);

          // Reset to 0,0,0; no rotation, no scaling.
          glLoadIdentity();
		
		unlockGL();

		Camera* temp_cam = d->getOSWindow()->getCamera(); 
          if( temp_cam )
			temp_cam->callGluLookAt( d );

		lockGL();
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unlockGL();
	}
	
	void Renderer::finishRender(Device* d){
		// Nothing here
	}

	void Renderer::renderGui( Device* d ){
		//prepareText(d);
		//finishText(d);
	}


	// Debug Functions: DELETE
	void Renderer::prepareText( Device * d ) {
		lockGL();
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glScalef( 1.f, 1.f , 1.f);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glEnable( GL_TEXTURE_2D );
		
		unlockGL();
	}

	void Renderer::finishText( Device * d ) {
		lockGL();
		
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		
		unlockGL();
	}
	
	/////////////////////////////////////
	////  Thread Functions
		
	void Renderer::lockGL(){
		SDL_LockMutex( gl_mutex );
	}
		
	void Renderer::unlockGL(){
		SDL_UnlockMutex( gl_mutex );
	}
	

	


} // end namespace Kaleidoscope
