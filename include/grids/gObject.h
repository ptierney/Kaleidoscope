
#pragma once

#include <grids/define.h>
#include <grids/objectController.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/define.h>
#include <QMutex>

namespace Kaleidoscope {
  class Device;
  class Camera;	 
  class NoticeWindow;
}

namespace Grids{

	class Object {

	public:
		Object( Kal::Device*, Value* );
		
		GridsID getID();
		GridsID getRoom();

		// Gets the absolute Position, including parents
		virtual Vec3D getPosition( );
		virtual Vec3D getScale( );
		virtual Vec3D getRotation( );

		virtual Vec3D getLocalPosition();
		virtual Vec3D getLocalRotation();
		virtual Vec3D getLocalScale();		
		
		virtual void updatePosition( Kal::Device*, Vec3D );
		virtual void updateScale( Kal::Device*, Vec3D );
		virtual void updateRotation( Kal::Device*, Vec3D );

                virtual void updateAttr(Event*);

		// attr is a Grids::Value that holds all the information
		// specific to the particular object.
		// This information should exist in a shared state between the
		// computers on the network.
		Value* getAttr();
		void lockAttr();
		void unlockAttr();
		
		void addChild( Object* obj_ptr );

		virtual void drawAll( Kal::Device* ) = 0;
		virtual bool getVisibility() = 0;		

		friend class ObjectController;
		friend class Kal::Camera;

	signals:
		void error(int, QString);
		void notice(int, QString);

	protected:
		Object* getParent();
		GridsID getParentID();
		void setRoom( GridsID );
		void setParent(Object*);
		void setParentID( GridsID );
		Value* getAttrFromValue( Value* );
		GridsID getIDFromValue( Value* );
                GridsID getParentFromValue( Value* );
		
		std::vector< Object* > getChildren();
		std::vector< Object* > children;

		Value attr;

                /* These functions are called by the ObjectController
                 * When it receives a message GRIDS_UPDATE_OBJECT */
                virtual void setLocalPosition( Vec3D );
                virtual void setLocalRotation( Vec3D );
                virtual void setLocalScale( Vec3D );

	private:
		void setAttr( Value* );
		void setAttrFromValue( Value* );
		void setInitialPositions( Value* );
		
		void setID( GridsID );

		Object* parent;
		GridsID parent_id;
	 
		GridsID obj_id;
		GridsID obj_room;
		
		Vec3D position;
		Vec3D rotation;
		Vec3D scale;
		
		QMutex position_mutex;
		QMutex rotation_mutex;
		QMutex scale_mutex;		
		QMutex children_mutex;		
		QMutex attr_mutex;
		QMutex id_mutex;
		QMutex parent_mutex;
		QMutex room_mutex;

		////////////////////////
		// These will be deleted
		////////////////////////
		void setAttrPosition( Vec3D );
		void setAttrRotation( Vec3D );
		void setAttrScale( Vec3D );

		virtual Vec3D getAttrPosition(  );
		virtual Vec3D getAttrScale( );
		virtual Vec3D getAttrRotation( );
	};

} // end namespace Grids

