
#pragma once

#include <kaleidoscope/spaceObject.h>
#include <kaleidoscope/text.h>
#include <string>

namespace Kaleidoscope {
	class Device;

	class SpaceText : public SpaceObject, public Text {
	public:
		SpaceText( Device*, Grids::Value* );
		
		void draw( Device* );
		void drawSpaceText( Device* );

		void prepareSpaceText();
		void finishSpaceText();
		
		static GridsID requestCreate( Device*, std::string text, unsigned int size, Vector3 );

	};

} // end namespace Kaleidoscope
