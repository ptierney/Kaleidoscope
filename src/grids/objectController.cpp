
#include <map>

#include <QMutex>
#include <QMutexLocker>
#include <QMouseEvent>

#include <grids/objectController.h>
#include <grids/define.h>
#include <kaleidoscope/define.h>

#include <grids/gObject.h>
#include <grids/event.h>
#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/genericLinkItem.h>
#include <kaleidoscope/inputTextItem.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/spaceText.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/device.h>



namespace Grids {

    ObjectController::ObjectController(Kal::Device* d_, QWidget* parent)
        : QWidget(parent) {
        d = d_;
    }

    void ObjectController::registerObject( GridsID new_id, Object* new_ptr ){
        object_ids.push_back( new_id );
        id_ptr_map[ new_id ] = new_ptr;
        ptr_id_map[ new_ptr ] = new_id;
    }

    // Returns an object's ID given it's pointer
    GridsID ObjectController::getIDFromPointer( Object* obj_ptr ){
        if( pointerRegistered(obj_ptr) == false)
            return GridsID();
        else
            return ptr_id_map[ obj_ptr ];
    }

    // Returns an object's pointer given its ID
    Object* ObjectController::getPointerFromID( GridsID obj_id ){
        if( idRegistered(obj_id) == false)
            return NULL;
        else
            return id_ptr_map[ obj_id ];
    }

    void ObjectController::createObject( GridsID new_id, Event* evt ){
        if( !knownObject( new_id, evt ) )
            createGenericObject( new_id, evt );
    }

    bool ObjectController::idRegistered( GridsID obj_id ){
        id_ptr_iter = id_ptr_map.find(obj_id);

        if( id_ptr_iter == id_ptr_map.end() )
            return false;
        else
            return true;
    }

    bool ObjectController::pointerRegistered( Object* obj_ptr ){
        ptr_id_iter = ptr_id_map.find(obj_ptr);

        if( ptr_id_iter == ptr_id_map.end() )
            return false;
        else
            return true;
    }

    void ObjectController::deleteObjectFromID( GridsID obj_id ) {


    }

    // Thoughs: this function should be replaced by a hash.  Each object
    // "registers" both it's name and ... is this possible?
    bool ObjectController::knownObject( GridsID new_id, Event* evt ) {
        //d->getNoticeWindow()->write(0, tr("Testing known object"));

        std::string type = (*(evt->getArgsPtr()))[ "attr" ][ "type" ].asString();
        bool found = 0;

        //d->getNoticeWindow()->write(0, tr(">>") + tr(type.c_str()) + tr("<<"));

        if( type == "Camera" ){
            d->registerCamera(evt->getArgsPtr());
            found = 1;
        } else if( type == "InputText") {
            //d->getNoticeWindow()->write(0, tr("Found InputText"));
            Kal::InputTextItem::gridsCreate(d, evt);
            found = 1;
        } else if( type == "GenericNode") {
            Kal::GenericNodeItem::gridsCreate(d, evt);
            found = 1;
        } else if( type == "GenericLink") {
            Kal::GenericLinkItem::gridsCreate(d, evt);
            found = 1;
        }

        /*else if( type == "SpaceText" ) {
		    new Kal::SpaceText( d, evt->getArgsPtr() );
		    found = 1;
		    }*/

        return found;
    }

    void ObjectController::createGenericObject( GridsID new_id, Event* evt ){
        // Create a cube with a questionmark or something
    }

    void ObjectController::registerCamera( GridsID new_id, Event* evt ){
        // Each camera is created inside of a new window, therefore the device must
        // create the camera.
    }

    void ObjectController::updateObjectPosition( GridsID in_id, Vec3D pos ){
        Object* obj = getPointerFromID( in_id );

        if(obj)
            obj->setLocalPosition(pos);
    }

    void ObjectController::updateObjectRotation( GridsID in_id, Vec3D rot ){
        Object* obj = getPointerFromID( in_id );

        if( obj )
            obj->setLocalRotation( rot );
    }

    void ObjectController::updateObjectScale( GridsID in_id, Vec3D scl ){
        Object* obj = getPointerFromID( in_id );

        if(obj)
            obj->setLocalScale( scl );
    }

    void ObjectController::updateObjectAttr( GridsID in_id, Event* evt ){
        Object* obj = getPointerFromID(in_id);

        if(obj)
            obj->updateAttr(evt);
    }

    void ObjectController::mouseReleasedEvent(QMouseEvent* event) {
        detectSelectionThreaded(event->x(), event->y());
    }

    void ObjectController::detectSelectionThreaded(int mouse_x, int mouse_y){

    }

} // end namespace Grids
