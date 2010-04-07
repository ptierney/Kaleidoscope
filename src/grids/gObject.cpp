
#include <grids/gObject.h>
#include <grids/event.h>
#include <kaleidoscope/noticeWindow.h>
#include <grids/objectController.h>
#include <grids/interface.h>
#include <kaleidoscope/device.h>

#include <QMutexLocker>

#include <iostream>

namespace Grids {

  Object::Object( Kaleidoscope::Device* d, Value* in_value ) {
    parent = NULL;

    setID( getIDFromValue( *in_value ) );
    setParentID( getParentFromValue( *in_value) );

    d->getObjectController()->registerObject(getID(), this);

    setInitialPositions( *in_value );
    setAttrFromValue( *in_value );
  }

  Object::~Object(){
    // Nothing here
  }

  GridsID Object::getID() {
    return obj_id;
  }

  GridsID Object::id(){
    return obj_id;
  }

  GridsID Object::getRoom() {
    return obj_room;
  }

  Vec3D Object::getPosition(){
    Vec3D parents_position = Vec3D();

    Object* temp_parent = getParent();

    if(temp_parent)
      parents_position = temp_parent->getPosition( );

    return getLocalPosition() + parents_position;
  }

  Vec3D Object::getScale(){
    Vec3D parents_scale = Vec3D(1.f, 1.f, 1.f);

    if( parent )
      parents_scale = parent->getScale( );

    return getLocalScale() * parents_scale;
  }

  Vec3D Object::getRotation(){
    Vec3D parents_rot = Vec3D(0.f, 0.f, 0.f);

    if(parent)
      parents_rot = parent->getRotation( );

    return getLocalRotation() + parents_rot;
  }

  Vec3D Object::getLocalPosition(){
    return position_;
  }

  Vec3D Object::getLocalRotation(){
    return rotation_;
  }

  Vec3D Object::getLocalScale() {
    return scale_;
  }

  void Object::setLocalPosition( Vec3D pos ){
    position_ = pos;
  }

  void Object::setLocalRotation( Vec3D rot ){
    rotation_ = rot;
  }

  void Object::setLocalScale( Vec3D scl ){
    scale_ = scl;
  }

  void Object::updatePosition( Kaleidoscope::Device* d, Vec3D pos ){
    d->getInterface()->requestUpdatePosition( getID(), pos );
  }

  void Object::updateRotation( Kaleidoscope::Device* d, Vec3D rot ){
    d->getInterface()->requestUpdateRotation( getID(), rot );
  }

  void Object::updateScale( Kaleidoscope::Device* d, Vec3D scl ) {
    d->getInterface()->requestUpdateScale( getID(), scl );
  }

  void Object::updateAttr(Event* evt) {
    setAttrFromValue( evt->getArgs() );
  }

  // TODO: This is NOT threadsafe!!
  Object* Object::getParent(){
    return parent;
  }

  GridsID Object::getParentID(){
    return parent_id;
  }

  void Object::setParent( Object* parent_ptr ){
    parent = parent_ptr;
  }

  void Object::setParentID( GridsID in_parent_id ){
    parent_id = in_parent_id;
  }

  const std::vector<Object*>& Object::getChildren(){
    return children;
  }

  GridsID Object::getIDFromValue(const Value& val){
    return val["id"].asString();
  }

  GridsID Object::getParentFromValue(const Value& val){
    return getAttrFromValue(val)["parent"].asString();
  }

  void Object::setInitialPositions(const Value& val){
    if( !( val[ "pos" ].empty() ) ) {
      setLocalPosition( getVectorFromValue(val, "pos") );
    }

    if( !( val[ "rot" ].empty() ) ) {
      setLocalRotation( getVectorFromValue(val, "rot") );
    }

    if( !( val[ "scl" ].empty() ) ) {
      setLocalScale( getVectorFromValue(val, "scl") );
    }
  }

  /* Most importantly this checks to see if the numbers are in string form. */
  Vec3D Object::getVectorFromValue(const Value& val, std::string key) {
    Vec3D pos;

    /* TODO: Refractor so there's less copy paste. */
    /* Fuck the number was sent in quotes. */
    if( val[key][0u].isString() ){
      std::string pos_str = val[key][0u].asString();
      pos.X = atof( pos_str.c_str() );
    } else {
      pos.X = val[key][0u].asDouble();
    }

    if( val[key][1u].isString() ){
      std::string pos_str = val[key][1u].asString();
      pos.Y = atof( pos_str.c_str() );
    } else {
      pos.Y = val[key][1u].asDouble();
    }

    if( val[key][2u].isString() ){
      std::string pos_str = val[key][2u].asString();
      pos.Z = atof( pos_str.c_str() );
    } else {
      pos.Z = val[key][2u].asDouble();
    }

    return pos;
  }

  const Value& Object::getAttrFromValue(const Value& val){
    return val["attr"];
  }

  const Value& Object::getAttr(){
    return attr;
  }

  void Object::setAttr(const Value& new_attr){
    if(new_attr == NULL)
      attr = Value();
    else
      attr = Value(new_attr);
  }

  void Object::setAttrFromValue(const Value& new_attr){
    setAttr(new_attr["attr"]);
  }


  void Object::setID( GridsID new_id ){
    obj_id = new_id;
  }

  void Object::setRoom( GridsID new_room ){
    obj_room = new_room;
  }

  void Object::addChild( Object* obj_ptr ){
    children.push_back( obj_ptr );
  }


} // end namespace Grids
