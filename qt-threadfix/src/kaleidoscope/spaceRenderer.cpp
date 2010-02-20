
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/device.h>
#include <iostream>

namespace Kaleidoscope {

        SpaceRenderer::SpaceRenderer(Device* dev, Grids::Value* val)
		: Object(dev, val) {
		d = dev;
		initVars();
	}

        void SpaceRenderer::draw(Device* in_d) {
		/*glBegin( GL_LINES );
		glColor4f( 1.f, 1.f, 1.f, 1.f );
		glVertex3f( 0.f, 0.f, 0.f );
		glVertex3f( 100.f, 100.f, 100.f );		
		glEnd();*/
	}

	void SpaceRenderer::renderAll() {
                drawAll(d);
	}
	
	void SpaceRenderer::initVars(){
		Texture_On = false;
          Light_On = false;
          Alpha_Add = false;
          Blend_On = true;
          Filtering_On = false;
          Smooth_On = true;
	
		loadLights();
          loadTextModes();
	}

	void SpaceRenderer::loadLights(){
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

	void SpaceRenderer::loadTextModes(){
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

	void SpaceRenderer::setTextureOn( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Texture_On = in_bool; 
	}

	void SpaceRenderer::setLightOn( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Light_On = in_bool; 
	}

	void SpaceRenderer::setAlphaAdd( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Alpha_Add = in_bool; 
	}

	void SpaceRenderer::setBlendOn( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Blend_On = in_bool; 
	}

	void SpaceRenderer::setFilteringOn( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Filtering_On = in_bool; 
	}

	void SpaceRenderer::setSmoothOn( bool in_bool ) { 
		QMutexLocker lock(&gl_settings_mutex);
		Smooth_On = in_bool; 
	}

        void SpaceRenderer::prepareWindow(){
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


        void SpaceRenderer::prepareRender1() {
          bool text_on;
          bool light_on;
          bool al_on;
          bool blend_on;
          bool filt_on;
          bool smooth_on;

        gl_settings_mutex.lock();

          text_on = Texture_On;
          light_on = Light_On;
          al_on = Alpha_Add;
          blend_on = Blend_On;
          filt_on = Filtering_On;
          smooth_on = Smooth_On;

          gl_settings_mutex.unlock();
			
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
	}

        void SpaceRenderer::prepareRender2(){
                        lockGL();
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                unlockGL();

        }
	
	void SpaceRenderer::finishRender(){
		// Nothing here
	}

	void SpaceRenderer::renderGui(){
		//prepareText(d);
		//finishText(d);
	}


	// Debug Functions: DELETE
	void SpaceRenderer::prepareText() {
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

        void SpaceRenderer::finishText() {
		lockGL();
		
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		
		unlockGL();
	}
	
	/////////////////////////////////////
	////  Thread Functions
		
	void SpaceRenderer::lockGL(){
		gl_mutex.lock();
	}
		
	void SpaceRenderer::unlockGL(){
		gl_mutex.unlock();
	}
	
	QMutex* SpaceRenderer::getGLMutex(){
		return &gl_mutex;
	}

} // end namespace Kaleidoscope
