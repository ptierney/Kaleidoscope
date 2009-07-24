
#pragma once

#include <grids/vector3d.h>
#include <grids/vector2d.h>
#include <grids/irrMath.h>
#include <grids/matrix4.h>

#include <string>

#define GRIDS_SERVER "block.hardchats.com"
#define DEFAULT_SERVER "block.hardchats.com"

#define CALLBACK_SECONDS 10
#define KALEIDOSCOPE_DEBUG_LEVEL 4

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

namespace Kaleidoscope
{
	typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;
	typedef irr::core::matrix4 Matrix4;
	
	typedef std::string GridsID;
	
	const float GRAD_PI = irr::core::RADTODEG; // 180.0f / PI
	
	const int FPS = 1;
	const int MAYA = 2;
	
	const int ZOOM_FORWARD = 1;
	const int ZOOM_CENTER = 2;
}

namespace Kal = Kaleidoscope;
