
#pragma once

#include <grids/define.h>
#include <kaleidoscope/define.h>

#include <QMutex>
#include <QString>

namespace Kaleidoscope {
  class Device;
  class Camera;	 
  class NoticeWindow;
}

namespace Grids{
  class ObjectControler;
  class Event;

	class Object {

	public:
    Object( Kaleidoscope::Device*, Value* );
    virtual ~Object();
		
		GridsID getID();
    GridsID id();
		GridsID getRoom();

		// Gets the absolute Position, including parents
		virtual Vec3D getPosition( );
		virtual Vec3D getScale( );
		virtual Vec3D getRotation( );

		virtual Vec3D getLocalPosition();
		virtual Vec3D getLocalRotation();
		virtual Vec3D getLocalScale();		
		
    virtual void updatePosition( Kaleidoscope::Device*, Vec3D );
    virtual void updateScale( Kaleidoscope::Device*, Vec3D );
    virtual void updateRotation( Kaleidoscope::Device*, Vec3D );

    virtual void updateAttr(Event*);

		// attr is a Grids::Value that holds all the information
		// specific to the particular object.
		// This information should exist in a shared state between the
		// computers on the network.
    const Value& getAttr();
		
		void addChild( Object* obj_ptr );

		friend class ObjectController;
    friend class Kaleidoscope::Camera;

	protected:
		Object* getParent();
		GridsID getParentID();
		void setRoom( GridsID );
		void setParent(Object*);
		void setParentID( GridsID );
    const Value& getAttrFromValue(const Value&);
    GridsID getIDFromValue(const Value&);
    GridsID getParentFromValue(const Value&);
		
    const std::vector<Object*>& getChildren();
    std::vector<Object*> children;

		Value attr;

    /* These functions are called by the ObjectController
                 * When it receives a message GRIDS_UPDATE_OBJECT */
    virtual void setLocalPosition( Vec3D );
    virtual void setLocalRotation( Vec3D );
    virtual void setLocalScale( Vec3D );

	private:
    void setAttr(const Value&);
    void setAttrFromValue(const Value&);
    void setInitialPositions(const Value&);
    Vec3D getVectorFromValue(const Value& val, std::string key);
		
		void setID( GridsID );

		Object* parent;
		GridsID parent_id;

		GridsID obj_id;
		GridsID obj_room;
		
    Vec3D position_;
    Vec3D rotation_;
    Vec3D scale_;
	};

} // end namespace Grids

