
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
		placeRoom( d );

		lockAttr();
		buildRoom(d);
		unlockAttr();

                d->getRenderer()->addChild( this );
	}
	
	void Room::draw( Device* d ){
            //std::cerr << "Draw\n";
		glDraw( d );
	}
	
	void Room::create( Device* d, Grids::Value* val ){
		// nothing here
	}

	void Room::placeRoom( Device * d )
	{
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
		GridsID new_id = getID();

		float room_width = d->getSettings()->getRoomWidth();
		int num_lines = room_width;

		attr = Grids::Value();		
		// Lines Color
		attr[ "color" ][ 0u ][ 0u ] = 1.0f;
		attr[ "color" ][ 0u ][ 1u ] = 1.0f;
		attr[ "color" ][ 0u ][ 2u ] = 1.0f;
		attr[ "color" ][ 0u ][ 3u ] = 0.1f;

		attr[ "lines" ][ 0u ][ "indices" ] = Grids::Value();
		attr[ "lines" ][ 0u ][ "color" ] = 0u;

		attr[ "vertices" ] = Grids::Value();
		
		// i * 2 from -num_lines to num_lines --  + 4 Vertices
		for(int i = 0; i <= num_lines * 2; i += 4 ) {
			attr[ "vertices" ][ i ] = Grids::Value();

			attr[ "vertices" ][ i ][ 0u ] = (float)i - room_width;
			attr[ "vertices" ][ i ][ 1u ] = 0.0f;
			attr[ "vertices" ][ i ][ 2u ] = (float)-num_lines;

			attr[ "vertices" ][ i+1 ] = Grids::Value();

			attr[ "vertices" ][ i+1 ][ 0u ] = (float)i - room_width;
			attr[ "vertices" ][ i+1 ][ 1u ] = 0.0f;
			attr[ "vertices" ][ i+1 ][ 2u ] = (float)num_lines ;

			attr[ "vertices" ][ i+2 ] = Grids::Value();

			attr[ "vertices" ][ i+2 ][ 0u ] = (float)num_lines;
			attr[ "vertices" ][ i+2 ][ 1u ] = 0.0f;
			attr[ "vertices" ][ i+2 ][ 2u ] = (float)i - room_width;

			attr[ "vertices" ][ i+3 ] = Grids::Value();

			attr[ "vertices" ][ i+3 ][ 0u ] = (float)-num_lines;
			attr[ "vertices" ][ i+3 ][ 1u ] = 0.0f;
			attr[ "vertices" ][ i+3 ][ 2u ] = (float)i - room_width;
		}

		// Indices are ordered pairs ( or triplets, or quintuplets ) of numbers that indicate whicu vertices belong to which line, triangle, or quadrilateral

		int j = 0;

		for( int i = 0; i < num_lines + 1 ; i += 2 )
		{
			attr[ "lines" ][ 0u ][ "indices" ][ i ] = Grids::Value();

			attr[ "lines" ][ 0u ][ "indices" ][ i ][ 0u ] = j;
			attr[ "lines" ][ 0u ][ "indices" ][ i ][ 1u ] = j + 1;

			attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ] = Grids::Value();

			attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 0u ] = j + 2;
			attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 1u ] = j + 3;

			j += 4;
		}

		// Wall Color

		attr[ "color" ][ 1u ] = Grids::Value();
		attr[ "color" ][ 1u ][ 0u ] = 1.0f;
		attr[ "color" ][ 1u ][ 1u ] = 1.0f;
		attr[ "color" ][ 1u ][ 2u ] = 1.0f;
		attr[ "color" ][ 1u ][ 3u ] = 0.01f;

		// Outline Color -- faint white
		attr[ "color" ][ 2u ] = Grids::Value();
		attr[ "color" ][ 2u ][ 0u ] = 1.0f;
		attr[ "color" ][ 2u ][ 1u ] = 1.0f;
		attr[ "color" ][ 2u ][ 2u ] = 1.0f;
		attr[ "color" ][ 2u ][ 3u ] = 0.15f;

		attr[ "quads" ][ 0u ] = Grids::Value();
		attr[ "quads" ][ 0u ][ "indices" ] = Grids::Value();
		attr[ "quads" ][ 0u ][ "color" ] = 1; // Use color # 1 to shade this quad

		int k = attr[ "vertices" ].size();

		for( int i = k; i < k + 8; i++ ){
			attr[ "vertices" ][ i ] = Grids::Value();
		}

		attr[ "vertices" ][ k ][ 0u ] = -room_width;
		attr[ "vertices" ][ k ][ 1u ] = -room_width;
		attr[ "vertices" ][ k ][ 2u ] = -room_width;

		attr[ "vertices" ][ k + 1 ][ 0u ] = room_width;
		attr[ "vertices" ][ k + 1 ][ 1u ] = -room_width;
		attr[ "vertices" ][ k + 1 ][ 2u ] = -room_width;

		attr[ "vertices" ][ k + 2 ][ 0u ] = room_width;
		attr[ "vertices" ][ k + 2 ][ 1u ] = room_width;
		attr[ "vertices" ][ k + 2 ][ 2u ] = -room_width;

		attr[ "vertices" ][ k + 3 ][ 0u ] = -room_width;
		attr[ "vertices" ][ k + 3 ][ 1u ] = room_width;
		attr[ "vertices" ][ k + 3 ][ 2u ] = -room_width;

		attr[ "vertices" ][ k + 4 ][ 0u ] = -room_width;
		attr[ "vertices" ][ k + 4 ][ 1u ] = -room_width;
		attr[ "vertices" ][ k + 4 ][ 2u ] = room_width;

		attr[ "vertices" ][ k + 5 ][ 0u ] = room_width;
		attr[ "vertices" ][ k + 5 ][ 1u ] = -room_width;
		attr[ "vertices" ][ k + 5 ][ 2u ] = room_width;

		attr[ "vertices" ][ k + 6 ][ 0u ] = room_width;
		attr[ "vertices" ][ k + 6 ][ 1u ] = room_width;
		attr[ "vertices" ][ k + 6 ][ 2u ] = room_width;

		attr[ "vertices" ][ k + 7 ][ 0u ] = -room_width;
		attr[ "vertices" ][ k + 7 ][ 1u ] = room_width;
		attr[ "vertices" ][ k + 7 ][ 2u ] = room_width;

		for( int i = 0; i < 6; i++ ){
			attr[ "quads" ][ 0u ][ "indices" ][ i ] = Grids::Value();
		}

		attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = k + 0u; // Box top
		attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = k + 1u;
		attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = k + 2u;
		attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = k + 3u;

		attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = k + 4u; // Box bottom
		attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = k + 5u;
		attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = k + 6u;
		attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = k + 7u;

		attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 1u ] = k + 5u;
		attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 2u ] = k + 1u;
		attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 3u ] = k + 0u;

		attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 1u ] = k + 6u;
		attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 2u ] = k + 2u;
		attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 3u ] = k + 3u;

		attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 0u ] = k + 4u;
		attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 1u ] = k + 7u;
		attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 2u ] = k + 3u;
		attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 3u ] = k + 0u;

		attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 0u ] = k + 5u;
		attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 1u ] = k + 6u;
		attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 2u ] = k + 2u;
		attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 3u ] = k + 1u;
				
		// Do the faint white outline lines

		attr[ "lines" ][ 1u ][ "color" ] = 2u;

		for( int i = 0; i < 12; i++ ){
			attr[ "lines" ][ 1u ][ "indices" ][ i ] = Grids::Value();
		}

		attr[ "lines" ][ 1u ][ "indices" ][ 0u ][ 0u ] = k + 0u;
		attr[ "lines" ][ 1u ][ "indices" ][ 0u ][ 1u ] = k + 1u;

		attr[ "lines" ][ 1u ][ "indices" ][ 1u ][ 0u ] = k + 3u;
		attr[ "lines" ][ 1u ][ "indices" ][ 1u ][ 1u ] = k + 2u;

		attr[ "lines" ][ 1u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		attr[ "lines" ][ 1u ][ "indices" ][ 2u ][ 1u ] = k + 5u;

		attr[ "lines" ][ 1u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		attr[ "lines" ][ 1u ][ "indices" ][ 3u ][ 1u ] = k + 6u;

		attr[ "lines" ][ 1u ][ "indices" ][ 4u ][ 0u ] = k + 0u;
		attr[ "lines" ][ 1u ][ "indices" ][ 4u ][ 1u ] = k + 3u;

		attr[ "lines" ][ 1u ][ "indices" ][ 5u ][ 0u ] = k + 1u;
		attr[ "lines" ][ 1u ][ "indices" ][ 5u ][ 1u ] = k + 2u;

		attr[ "lines" ][ 1u ][ "indices" ][ 6u ][ 0u ] = k + 4u;
		attr[ "lines" ][ 1u ][ "indices" ][ 6u ][ 1u ] = k + 7u;

		attr[ "lines" ][ 1u ][ "indices" ][ 7u ][ 0u ] = k + 5u;
		attr[ "lines" ][ 1u ][ "indices" ][ 7u ][ 1u ] = k + 6u;

		attr[ "lines" ][ 1u ][ "indices" ][ 8u ][ 0u ] = k + 4u;
		attr[ "lines" ][ 1u ][ "indices" ][ 8u ][ 1u ] = k + 0u;

		attr[ "lines" ][ 1u ][ "indices" ][ 9u ][ 0u ] = k + 7u;
		attr[ "lines" ][ 1u ][ "indices" ][ 9u ][ 1u ] = k + 3u;

		attr[ "lines" ][ 1u ][ "indices" ][ 10u ][ 0u ] = k + 5u;
		attr[ "lines" ][ 1u ][ "indices" ][ 10u ][ 1u ] = k + 1u;

		attr[ "lines" ][ 1u ][ "indices" ][ 11u ][ 0u ] = k + 6u;
		attr[ "lines" ][ 1u ][ "indices" ][ 11u ][ 1u ] = k + 2u;
				
	} // end BuildRoom
} // end namespace Kaleidoscope
