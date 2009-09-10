
#pragma once

#include <kaleidoscope/object.h>
#include <kaleidoscope/camera.h>
#include <QGLWidget>

namespace Kaleidoscope {
	class Device;
	
	// Everything that should be drawn should be a child of this object
	// Maybe this should be split into a placeholder (GridsParent) and 
	// a strictly OpenGL drawer.
	class SpaceRenderer : public Object {

	public:
		SpaceRenderer(Device*, Grids::Value*);	
		
		void renderAll();
		
                void draw(Device*);
		
		void lockGL();
		void unlockGL();
		QMutex* getGLMutex();

		friend class Camera;
		friend class Device;

	private:
		void prepareWindow();
		void prepareRender();
		void finishRender();

		//void resizeScene(Device*, unsigned int new_width, unsigned int new_height);
		void renderGui();

		void initVars();
		void loadLights();
		void loadTextModes();
		
		void setTextureOn( bool );
		void setLightOn( bool );
		void setAlphaAdd( bool );
		void setBlendOn( bool );
		void setFilteringOn( bool );
		void setSmoothOn( bool );

		bool Texture_On;
                bool Light_On;
                bool Alpha_Add;
                bool Blend_On;
                bool Filtering_On;
                bool Smooth_On;
		
		float Light_Ambient[4];
		float Light_Diffuse[4];
		float Light_Position[4];
		
		QMutex gl_mutex;
		QMutex gl_settings_mutex;

		Device* d;

		int current_text_mode;
		std::string text_mode_string[4];
                GLint text_modes[4];

		// Debug Functions
                void prepareText();
                void finishText();
	};

} // end namespace Kaleidoscope
