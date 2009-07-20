
#pragma once

#include <grids/vector3d.h>
#include <grids/vector2d.h>
#include <grids/irrMath.h>
#include <grids/matrix4.h>

#include <string>

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
