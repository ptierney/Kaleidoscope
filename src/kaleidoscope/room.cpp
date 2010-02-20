
#include <kaleidoscope/room.h>
#include <kaleidoscope/glSpaceObject.h>
#include <grids/define.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/settings.h>
#include <kaleidoscope/spaceRenderer.h>
#include <grids/interface.h>
#include <iostream>

namespace Kaleidoscope {

    Room::Room( Device* d, Grids::Value* val ) : GLSpaceObject( d, val ) {
        /* placeRoom( d ); */

        /* This object derives from Grids::Object, and has a setAttr(Grids::Value*) method
		   that deals with multithreading issues.
		   In actuality, this should append the geometry to attr */
        /*
		lockAttr();
		buildRoom(d);
		unlockAttr();
                */

        d->getRenderer()->addChild( this );
    }

    void Room::draw( Device* d ){
        /*glDraw( d );*/
    }

    void Room::create( Device* d, Grids::Value* val ){
        // nothing here
    }

    void Room::placeRoom( Device * d ) {
        GridsID new_id = getID();

        float room_width = d->getSettings()->getRoomWidth();

        bool rooms_dont_exist = d->getInterface()->getKnownRooms().empty();

        if( rooms_dont_exist ){
            setLocalPosition( Vec3D( 0.0f, 0.0f, 0.0f ) );
            setLocalRotation( Vec3D( 0.0f, 0.0f, 0.0f ) );
            setLocalScale( Vec3D( 1.0f, 1.0f, 1.0f ) );
        }
        else{
            int world_size = 5; // 20 x 20 x 20 = 8000 total rooms

            // The default position is the position to place new rooms if all of the rooms are filled up
            // It is the farthest possible position
            Vec3D default_position = Vec3D( -world_size * room_width * 2,
                                            -world_size * room_width * 2,
                                            -world_size * room_width * 2 );

            Vec3D closest_position = default_position;

            Vec3D temp_position;

            for( int i = -world_size; i < world_size; i++) {
                for( int g = -world_size; g < world_size; g++) {
                    for( int h = -world_size; h < world_size; h++) {

                        temp_position = Vec3D( i * 2 * room_width, g * 2 * room_width, h * 2 * room_width );
                        int j_max = d->getInterface()->getKnownRooms().size();

                        for( int j = 0; j < j_max; j++ ) {
                            GridsID temp_id = d->getInterface()->getKnownRooms()[ j ];

                            Grids::Object* temp_object = d->getObjectController()->getPointerFromID( temp_id );
                            Vec3D temp_room_position = temp_object->getPosition();

                            if( temp_position == temp_room_position ){
                                temp_position = default_position;
                            }
                        } // end for Rooms

                        if( temp_position.getLength() < closest_position.getLength() ){
                            closest_position = temp_position;
                        }
                    } // end for h
                } // end for g
            } // end for i

            setLocalPosition( closest_position );
            setLocalScale( Vec3D( 1.0f, 1.0f, 1.0f ) );
            setLocalRotation( Vec3D( 0.0f, 0.0f, 0.0f ) );
        } // end else
    } // end placeRoom

    void Room::buildRoom( Device * d ) {


    } // end BuildRoom
} // end namespace Kaleidoscope
