
#include <kaleidoscope/text.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/utility.h>
#include <iostream>

namespace Kaleidoscope {

	Text::Text() {
		// TTF_Init() called in OSWindow constructor, after setting up SDL
		text_mutex = SDL_CreateMutex();
		text_surface = 0;
		text_font = 0;

		glGenTextures( 1, &texture );
	}
	
	Text::~Text() {
		// TTF_Quit() called in OSWindow destructor
		if( text_font )
			TTF_CloseFont( text_font );
	}
	
	bool Text::loadFont( std::string font, unsigned int point_size ){
		if( text_font ){
			TTF_CloseFont( text_font );
			text_font = 0;
		}
		
		text_font = TTF_OpenFont( font.c_str(), point_size );
		text_font ? Utility::puts( 2, "Loaded font from: ", font ) : Utility::puts( 0, "Could not load font from: ", font ); 

		return text_font ? 1 : 0;
	}

	unsigned int Text::getFontSizeFromAttr( Grids::Value* attr ){
		return (*attr)["size"].asUInt();
	}
	
	std::string Text::getTextFromAttr( Grids::Value* attr ){
		return (*attr)["text"].asString();
	}

	// Billboard Spherical Rotates the object in two axis to always face the camera
	void Text::billboardSphericalBegin( float camX, float camY, float camZ,
								 float objPosX, float objPosY, float objPosZ) {

		float angleCosine;

		// objToCamProj is the vector in world coordinates from the
		// local origin to the camera projected in the XZ plane

		// This is the original lookAt vector for the object
		// in world coordinates

		// normalize both vectors to get the cosine directly afterwards
		Vec3D objToCamProjVec = Vec3D( camX - objPosX, 0.0f, camZ - objPosZ );
		Vec3D lookAtVec = Vec3D( 0.0f, 0.0f, 1.0f );

		objToCamProjVec.normalize();

		// easy fix to determine wether the angle is negative or positive
		// for positive angles upAux will be a vector pointing in the
		// positive y direction, otherwise upAux will point downwards
		// effectively reversing the rotation.

		Vec3D upAuxVec = lookAtVec.crossProduct( objToCamProjVec );

		// compute the angle
		angleCosine = lookAtVec.dotProduct( objToCamProjVec ) ;

		// perform the rotation. The if statement is used for stability reasons
		// if the lookAt and objToCamProj vectors are too close together then
		// |angleCosine| could be bigger than 1 due to lack of precision
		//if ( (angleCosine < 0.9999) && (angleCosine > -0.9999) )
		{
			glRotatef( acos(angleCosine)*180/irr::core::PI, upAuxVec.X, upAuxVec.Y, upAuxVec.Z );
		}

		// so far it is just like the cylindrical billboard. The code for the
		// second rotation comes now
		// The second part tilts the object so that it faces the camera

		// objToCam is the vector in world coordinates from
		// the local origin to the camera

		Vec3D objToCamVec = Vec3D( camX - objPosX, camY - objPosY, camZ - objPosZ );

		// Normalize to get the cosine afterwards

		objToCamVec.normalize();

		// Compute the angle between objToCamProj and objToCam,
		//i.e. compute the required angle for the lookup vector

		angleCosine = objToCamProjVec.dotProduct( objToCamVec ) ;

		// Tilt the object. The test is done to prevent instability
		// when objToCam and objToCamProj have a very small
		// angle between them

		if ( (angleCosine < 0.99990) && (angleCosine > -0.9999) )
			{
				if (objToCamVec.Y < 0)
					{
						glRotatef(acos(angleCosine)*180/irr::core::PI,1,0,0);
					}
				else
					{
						glRotatef(acos(angleCosine)*180/irr::core::PI,-1,0,0);
					}
			}
	}

	void Text::billboardEnd(){
		// restore the previously
		// stored modelview matrix
		//glPopMatrix();
	}

	void Text::setText( std::string new_text ){
		if( text_surface )
			SDL_FreeSurface( text_surface );
		
		text_surface = 0;
		text_surface = createSDLTextSurface( new_text, text_font );
		
		if( text_surface )
			text = new_text;
	}

	std::string Text::getText(){
		return text;
	}

	SDL_Surface* Text::createSDLTextSurface( std::string in_string, TTF_Font* in_font ){
		SDL_Surface * new_text_surface_1 = TTF_RenderText_Blended( in_font, in_string.c_str(), text_color );

		// Power of 2 optimization 		
		int x = 1;
		while(x < new_text_surface_1->w){
			x*=2;
		}

		int y = 1;
		while(y < new_text_surface_1->h){
			y*=2;
		}

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				Uint32 rmask = 0xff000000;
				Uint32 gmask = 0x00ff0000;
				Uint32 bmask = 0x0000ff00;
				Uint32 amask = 0x000000ff;
		#else
				Uint32 rmask = 0x000000ff;
				Uint32 gmask = 0x0000ff00;
				Uint32 bmask = 0x00ff0000;
				Uint32 amask = 0xff000000;
		#endif

		SDL_Surface * new_text_surface_2 = SDL_CreateRGBSurface( SDL_HWSURFACE, x, y, 32, rmask, gmask, bmask, amask);
		// SDL_HWSURFACE -- create the text in video memory
		// SDL_SWSURFACE -- create the text in system memory

		SDL_SetAlpha( new_text_surface_1, 0, 0);

		SDL_BlitSurface(new_text_surface_1, 0, new_text_surface_2, 0);
		//SDL_FreeSurface( new_text_surface_1 );
		SDL_FreeSurface( new_text_surface_2 );
		return new_text_surface_1;
	}


} // end namespace Kaleidoscope
