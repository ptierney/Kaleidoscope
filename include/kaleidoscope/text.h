
#pragma once

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_thread.h>
// Included for SDL_Color
#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>
#include <string>
#include <grids/define.h>

namespace Kaleidoscope {
	class Device;

	class Text {
	public:
		std::string getText();

	protected:
		Text();
		~Text();

		bool loadFont( std::string name, unsigned int point_size );

		unsigned int getFontSizeFromAttr( Grids::Value* );
		std::string getTextFromAttr( Grids::Value* );
		
		void setText( std::string );

		//void drawScreenText( Device* ); 

		//void prepareScreenText( Device* );
		//void finishScreenText( Device* );

		// camX, camY, camZ,  objPosX, Y, Z
		void billboardSphericalBegin( float, float, float, float, float, float );
		void billboardEnd();

		void clearText( Device *, int, int );
		SDL_Surface* createSDLTextSurface( std::string in_string, TTF_Font* in_font );		

		SDL_Surface* text_surface;
		TTF_Font* text_font;
		GLuint texture;
		SDL_Color text_color;

	private:
		SDL_mutex* text_mutex; 
		std::string text;
		
	};

} // end namespace Kaleidoscpe
