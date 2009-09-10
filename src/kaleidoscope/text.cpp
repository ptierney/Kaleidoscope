
#include <kaleidoscope/text.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/utility.h>
#include <iostream>

namespace Kaleidoscope {

	Text::Text() {
		// TTF_Init() called in OSWindow constructor, after setting up SDL
		text_surface = 0;
		text_font = 0;

		glGenTextures( 1, &texture );
	}
	
	Text::~Text() {
	}
	
	bool Text::loadFont( std::string font, unsigned int point_size ){
		if( text_font ){
			text_font = 0;
		}
		
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

	}

	std::string Text::getText(){
		return text;
	}

} // end namespace Kaleidoscope
