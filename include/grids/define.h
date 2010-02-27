
#pragma once

#include <string>
#include <map>
#include <iterator>
#include <queue>

#include <grids/vector3d.h>
#include <grids/vector2d.h>


#include <json/value.h>


#define GRIDS_ID_ERROR "ID ERROR"
#define GRIDS_CREATE_ROOM "Room.Create" 
#define GRIDS_CREATE_OBJECT "Room.CreateObject"
#define GRIDS_UPDATE_OBJECT "Room.UpdateObject"
#define GRIDS_LIST_ROOMS "Room.List"

#define GRIDS_NULL_EVENT "NULL_EVENT"

namespace Grids {

        typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;
	typedef irr::core::vector3df Vector3;
        typedef irr::core::vector2df Vector2;
/*
    typedef irr::core::vector3d<qreal> Vec3D;
    typedef irr::core::vector3d<qreal> Vector3;
    typedef irr::core::vector2d<qreal> Vec2D;
    typedef irr::core::vector2d<qreal> Vector2;
    */

        /* EventQueue typedef is in event.h */

	typedef std::string GridsID;
	
	typedef Json::Value Value;

        const int BROADCAST = 1;
        const int LOCAL = 0;

}


