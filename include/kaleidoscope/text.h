
#pragma once

#include <string>
#include <grids/define.h>

namespace Kaleidoscope {
	class Device;

	class Text {
	public:
    //std::string getText();

	protected:
		Text();
		~Text();

    //bool loadFont( std::string name, unsigned int point_size );

    //unsigned int getFontSizeFromAttr( Grids::Value* );
    //std::string getTextFromAttr( Grids::Value* );
		
    //void setText( std::string );

		//void drawScreenText( Device* ); 

		//void prepareScreenText( Device* );
		//void finishScreenText( Device* );

		// camX, camY, camZ,  objPosX, Y, Z
    //void billboardSphericalBegin( float, float, float, float, float, float );
    //void billboardEnd();

    //void clearText( Device *, int, int );


	private: 
    //std::string text;
		
	};

} // end namespace Kaleidoscpe
