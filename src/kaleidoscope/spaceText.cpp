
#include <kaleidoscope/spaceText.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/spaceRenderer.h>
#include <grids/interface.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

//	SpaceText::SpaceText( Device* d, Grids::Value* val ) : SpaceObject( d, val ) {
  //  SpaceText::SpaceText() {
    // Get font name and point size from attr
		// load font with loadFont
                //Grids::Value* attr = getAttrFromValue( val );
		
		// loadFont MUST be called before set text
                //loadFont( "media/Helvetica LT 55 Roman.ttf", getFontSizeFromAttr( attr ) );
                //setText( getTextFromAttr( attr ) );
		
		//Get my room, add this as a child to the room
                //Grids::Object* temp_room = d->getObjectController()->getPointerFromID( d->getMyRoom() );
                //if( temp_room )
                //	temp_room->addChild( this );
                //else
                 //       d->getRenderer()->addChild( this );
        //}
/*
	void SpaceText::draw( Device* d ){
                //prepareSpaceText();
                //drawSpaceText( d );
                //finishSpaceText();
	}

	// This text floats in 3D space, though always faces the camera.  Billboarded text.
	void SpaceText::drawSpaceText( Device * d ){
		int temp_x;
		int temp_y;
		float temp_scalar;
                Vec3D cam_position = d->getCamera()->getPosition();
		Vec3D text_position = getPosition();
		Vec3D text_scale = getScale();
                GLuint texture;

		float cam_x = cam_position.X;
		float cam_y = cam_position.Y;
		float cam_z = cam_position.Z;
			
		float pos_x = text_position.X;
		float pos_y = text_position.Y;
		float pos_z = text_position.Z;

                temp_x = 0;
                temp_y = 0;

		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );

                //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, text_surface->w, text_surface->h, 0, GL_RGBA,
                //		    GL_UNSIGNED_BYTE, text_surface->pixels );

		glColor4f( 1.0f, 1.0f, 1.0f, 0.75f );

		glPushMatrix();

		glTranslatef( pos_x, pos_y, pos_z );

		billboardSphericalBegin(cam_x, cam_y, cam_z, pos_x, pos_y, pos_z );
		
		temp_scalar = text_position.getDistanceFrom( cam_position );

		//std::cout << temp_scalar << std::endl;
		temp_scalar *= 0.000000001f;

		//std::cout << temp_scalar << std::endl;

		glScalef( temp_scalar, temp_scalar, temp_scalar);
		glScalef( text_scale.X, text_scale.Y, text_scale.Z);

		glBegin(GL_QUADS); {
			glTexCoord2i(0,0); glVertex2i(-temp_x, temp_y );
			glTexCoord2i(1,0); glVertex2i( temp_x, temp_y );
			glTexCoord2i(1,1); glVertex2i( temp_x, -temp_y );
			glTexCoord2i(0,1); glVertex2i(-temp_x, -temp_y );
		} glEnd();

		glDeleteTextures( 1, &texture );

		billboardEnd();

		glPopMatrix();
	}

	void SpaceText::prepareSpaceText( ){
		glPushMatrix();

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glEnable( GL_TEXTURE_2D );

		glBlendFunc( GL_SRC_ALPHA, GL_ONE);
	}

	void SpaceText::finishSpaceText(){
		glPopMatrix();
	}

	GridsID SpaceText::requestCreate( Device* d, std::string text, unsigned int size, Vec3D position ){
		Grids::Value attr; 
		
		attr["type"] = "SpaceText";
		attr["text"] = text;
		attr["size"] = size;
	
		return d->getInterface()->requestCreateObject( &attr, position, Vector3(0.f, 0.f, 0.f), Vector3( 1.f, 1.f, 1.f )  ); 
	}
*/
} // end namespace  Kaleidoscope
