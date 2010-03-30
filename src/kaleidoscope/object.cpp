
#include <kaleidoscope/object.h>
#include <kaleidoscope/define.h>
#include <grids/gObject.h>
#include <QMutex>
#include <QMutexLocker>

namespace Kaleidoscope {

	Object::Object(Device* d, Grids::Value* val) : Grids::Object( d, val ) {
		show();
	}
	
	void Object::drawAll(Device* d){
          if( getVisibility() ){
			std::vector< Grids::Object* > temp_children = getChildren();

               for( unsigned int i = 0u; i < temp_children.size(); i++ ){
                    //temp_children[i]->drawAll( d );
               }
               draw( d ); // draw yourself
          }
	}

	bool Object::detectSelection(){
    return false;
	}

	////////////////////////////
	// Accessor functions

	void Object::show(){
                //QMutexLocker lock(&visible_mutex);
		visible = 1;
	}

	void Object::hide(){
                //QMutexLocker lock(&visible_mutex);
		visible = 0;
	}

	bool Object::getVisibility(){
                //QMutexLocker lock(&visible_mutex);
		return visible;
	}

	void Object::select(){
                //QMutexLocker lock(&selected_mutex);
		selected = 1;
	}	

	void Object::deselect(){
                //QMutexLocker lock(&selected_mutex);
		selected = 0;
	}

} // end namespace Kaleidoscope
