
#include <iostream>

#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>

#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/frameRect.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/linkCreator.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  TeteNode::TeteNode(Device* d, QGraphicsItem* parent, QGraphicsScene* scene) :
    RespondNode(d, parent, scene) {
    tete_ = NULL;
    x_vel_ = 0.0;
    y_vel_ = 0.0;
    frame_rect_ = QRectF(-1, -1, 2, 2);
    frame_rect_object_ = NULL;
    mouse_moved_ = true;
    velocity_ = Vec3D();
    dormant_ = false;
    frame_on_needed_ = false;
    frame_on_threshold_ = 1.001;
    setFlag(QGraphicsItem::ItemIsMovable);
    //setAcceptDrops(true); // Needed for drag and drop events (like dragEnterEvent)
  }

  TeteNode::~TeteNode(){
    if(tete_)
      tete_->set_tete_node(NULL);
    delete menu_;
  }

  void TeteNode::init(){
    RespondNode::init();

    last_active_.start();

    menu_ = new QMenu(tr("Popup"));
    create_new_link_ = new QAction(tr("Create new link"), this);
    connect(create_new_link_, SIGNAL(triggered()),
            this, SLOT(registerLinkCreate()));
    menu_->addAction(create_new_link_);
    cancel_link_create_ = new QAction(tr("Cancel link creation"), this);
    connect(cancel_link_create_, SIGNAL(triggered()),
            this, SLOT(cancelLinkCreate()));
    menu_->addAction(cancel_link_create_);
    focus_on_user_ = new QAction(tr("Focus on this person"), this);
    connect(focus_on_user_, SIGNAL(triggered()),
            this, SLOT(focusOnUser()));
    menu_->addAction(focus_on_user_);
    user_last_tete_ = new QAction(tr("See person's last message"), this);
    connect(user_last_tete_, SIGNAL(triggered()),
            this, SLOT(focusOnLastUserTete()));
    menu_->addAction(user_last_tete_);
    playback_ = new QAction(tr("Playback chat from here"), this);
    connect(playback_, SIGNAL(triggered()),
            this, SLOT(playbackFromHere()));
    menu_->addAction(playback_);

    updateFrameRect();
  }

  Tete* TeteNode::tete(){
    return tete_;
  }

  void TeteNode::set_tete(Tete* tete){
    tete_ = tete;
  }

  void TeteNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    selected_ = true;
    beginFraming();
    QGraphicsObject::hoverEnterEvent(event);
  }

  void TeteNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    selected_ = false;
    QGraphicsObject::hoverLeaveEvent(event);
  }

  void TeteNode::beginFraming(){
    updateFrameRect();
    frame_selected_ = true;
    mouse_moved_ = false;
    frame_on_needed_ = true;
    //d_->chat_controller()->set_last_selected(tete_);
    if(frame_rect_object_){
      delete frame_rect_object_;
      frame_rect_object_ = NULL;
    }
    frame_rect_object_ = new FrameRect(d_, frame_rect_, this);
    d_->getScene()->addItem(frame_rect_object_);
    activateLinkedNodes();
  }

  bool TeteNode::frameOn(){
    if(frame_on_needed_ == false)
      return selected_;

    activate();
    // Get the rects of parent, referenced, and children nodes
    updateFrameRect();
    float total_object_width = frame_rect_.width();
    float total_object_height = frame_rect_.height();
    //QString string;
    //string.setNum(total_object_width);
    //d_->getNoticeWindow()->write(7, tr("width = ")+string);
    //string.setNum(total_object_height);
    //d_->getNoticeWindow()->write(7, tr("height = ")+string);

    float window_width = d_->main_window->centralWidget()->size().width();
    float window_height = d_->main_window->centralWidget()->size().height();

    float display_width = window_width - zoom_margin_*2.0;
    float display_height = window_height - zoom_margin_*2.0;

    float view_scale_width = display_width / total_object_width;
    float view_scale_height = display_height / total_object_height;

    float view_scale = view_scale_width <= view_scale_height ?
                       view_scale_width :
                       view_scale_height;    

    QMatrix current_matrix = d_->getScene()->main_view()->matrix();
    // Note: m11 and m22 hold the horizontal and vertical scale.
    // They should be the same.
    float current_scale = current_matrix.m11();
    float new_scale = current_scale + (view_scale - current_scale) * zoom_speed_;

    /* Removed to force a refresh of the screen
    if(new_scale / current_scale < frame_on_threshold_)
      frame_on_needed_ = false;
      */

    QMatrix temp_matrix;
    temp_matrix.scale(new_scale, new_scale);
    d_->getScene()->main_view()->setMatrix(temp_matrix);

    //d_->getScene()->main_view()->ensureVisible(frame_rect_, zoom_margin_, zoom_margin_);
    d_->getScene()->main_view()->centerOn(this);

    if(mouse_moved_ == false){
      mouse_moved_ = true;

      QPoint condense = d_->getScene()->main_view()->mapToGlobal(d_->getScene()->main_view()->mapFromScene(pos().toPoint()));
      d_->getScene()->main_view()->cursor().setPos(condense);
    }

    return selected_;
  }

  void TeteNode::frameLeave(FrameRect* /*frame*/){

  }

  bool TeteNode::frame_selected(){
    if(frame_rect_object_ == NULL)
      return false;

    if(frame_selected_ == false)
      return false;

    updateFrameSelected();

    return frame_selected_;
  }

  // TODO: frame selected should be a pure accessor
  // method, and put the other code in a more
  // properly labeled method.
  bool TeteNode::get_frame_selected(){
    return frame_selected_;
  }

  void TeteNode::updateFrameSelected(){
    frame_selected_ = frame_rect_object_->isUnderMouse();

    if(frame_selected_ == false){
      delete frame_rect_object_;
      frame_rect_object_ = NULL;
    }
  }

  void TeteNode::updatePosition(){
    setPos(QPointF( x() + velocity_.X + tete_->chat()->chat_node()->velocity().X,
                    y() + velocity_.Y + tete_->chat()->chat_node()->velocity().Y) );

    //setPos(x() + velocity_.X + tete_->chat()->chat_node()->pos().x(),
    //       y() + velocity_.Y + tete_->chat()->chat_node()->pos().y() );
  }

  float TeteNode::x_vel(){
    return velocity_.X;
  }

  float TeteNode::y_vel(){
    return velocity_.Y;
  }

  Vec3D TeteNode::velocity(){
    return velocity_;
  }

  void TeteNode::set_velocity(Vec3D velocity){
    velocity_ = velocity;
  }

  void TeteNode::addVelocity(Vec3D addition){
    velocity_ += addition;
  }

  QRectF TeteNode::frame_rect(){
    return frame_rect_;
  }

  void TeteNode::updateFrameRect(){
    // First set the initial min max values to this object's bounding
    // box
    QRectF local_bound = boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(pos());
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    float start_min_x, start_min_y, start_max_x, start_max_y;
    float min_x = start_min_x = bound.topLeft().x();//frame_rect_.topLeft().x();
    float min_y = start_min_y = bound.topLeft().y();//frame_rect_.topLeft().y();
    float max_x = start_max_x = bound.bottomRight().x();//frame_rect_.bottomRight().x();
    float max_y = start_max_y = bound.bottomRight().y();//frame_rect_.bottomRight().y();

    std::vector<Link*> links = tete_->links();
    Tete* other_node;
    // Should this be a const_iterator?
    for(std::vector<Link*>::const_iterator it = links.begin(); it != links.end(); ++it){
      if((*it)->node_1() == tete_)
        other_node = (*it)->node_2();
      else
        other_node = (*it)->node_1();

      addTeteToMinMax(other_node, min_x, min_y,
                      max_x, max_y);
    }

    // If nothing was added to one side, double the frame on that side,
    // so that the center node is more or less guaranteed to be centered.
    if(min_x == start_min_x)
      min_x -= (max_x - min_x);
    if(min_y == start_min_y)
      min_y -= (max_y - min_y);
    if(max_x == start_max_x)
      max_x += (max_x - min_x);
    if(max_y == start_max_y)
      max_y += (max_y - min_y);

    frame_rect_ = QRectF(QPointF(min_x, min_y),
                         QPointF(max_x, max_y)).normalized();
  }

  void TeteNode::addTeteToMinMax(Tete* tete,
                          float& min_x, float& min_y,
                          float& max_x, float& max_y) {
      //return;
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    QRectF local_bound = tete->tete_node()->boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(tete->tete_node()->pos());
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    if(bound.topLeft().x() < min_x)
      min_x = bound.topLeft().x();
    if(bound.topLeft().y() < min_y)
      min_y = bound.topLeft().y();
    if(bound.bottomRight().x() > max_x)
      max_x = bound.bottomRight().x();
    if(bound.bottomRight().y() > max_y)
      max_y = bound.bottomRight().y();
  }

  QTime& TeteNode::last_active(){
    return last_active_;
  }

  int TeteNode::activeElapsed(){
    return last_active_.elapsed();
  }

  bool TeteNode::dormant(){
    return dormant_;
  }

  void TeteNode::activate(){
    sendActivate();
    dormant_ = false;
    last_active_.start();
  }

  void TeteNode::receiveActivate(double /*amount*/){
    dormant_ = false;
    last_active_.start();
  }

  void TeteNode::sendActivate(){
    if(last_active_.elapsed() > 1000)
      Tete::requestUpdate(d_, tete_->id(), 1.0);
  }

  void TeteNode::activateLinkedNodes(){
    const std::vector<Link*>& links = tete_->links();

    for(std::vector<Link*>::const_iterator it = links.begin(); it != links.end(); ++it){
      if((*it)->node_1()->tete_node() != this)
        (*it)->node_1()->tete_node()->activate();

      if((*it)->node_2()->tete_node() != this)
        (*it)->node_2()->tete_node()->activate();

      (*it)->link_node()->activate();
    }
  }

  QPointF TeteNode::getGlobalPosition(){
    return d_->getScene()->main_view()->mapToGlobal(d_->getScene()->main_view()->mapFromScene(pos().toPoint()));
  }

  QColor TeteNode::gradient_color(){
    return gradient_color_;
  }

  void TeteNode::set_gradient_color(QColor new_color){
    gradient_color_ = new_color;
  }

  // Note: this is not called by the Qt event loop, it is called by whichever
  // derived class implements mouseDoubleClickEvent
  void TeteNode::mousePressEvent(QGraphicsSceneMouseEvent* event){
    mouse_down_ = event->pos() + pos();
    //mouse_down_vec_ = Vec3D(mouse_down_.x(), mouse_down_.y(), 0.0);
    //std::cerr << "Press: " << mouse_down_.x() << " : " << mouse_down_.y() << std::endl;
    QGraphicsObject::mousePressEvent(event);
  }

  void TeteNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    QPointF scene_pos = event->pos() + pos();
    //std::cerr << "Release: " << scene_pos.x() << " : " << scene_pos.y() << std::endl;
    //Vec3D temp_vec = Vec3D(event->pos().x(), event->pos().y(), 0.0);
    if(mouse_down_ == scene_pos) {
      popupMenu();
    } else {
      //std::cerr << "Probably drag" << std::endl;
      for(std::vector<Link*>::const_iterator it = tete_->links().begin(); it != tete_->links().end(); ++it){
        if((*it)->link_node() != NULL)
          (*it)->link_node()->update();
      }
      update();
    }
    QGraphicsObject::mouseReleaseEvent(event);
  }

  // These two function don't get called. I don't know why.
  void TeteNode::dragEnterEvent(QGraphicsSceneDragDropEvent* event){
    std::cerr << qrand() << " Drag Enter" << std::endl;
    QGraphicsObject::dragEnterEvent(event);
  }

  void TeteNode::dragMoveEvent(QGraphicsSceneDragDropEvent* event){
    d_->getScene()->update();
    update();
    std::cerr << qrand() << " Drag Move" << std::endl;
    QGraphicsObject::dragMoveEvent(event);
  }

  void TeteNode::set_mouse_moved(bool mouse){
    mouse_moved_ = mouse;
  }

  void TeteNode::popupMenu() {
    QPoint mouse_global = d_->getScene()->main_view()->cursor().pos();
    menu_->popup(mouse_global);
  }

  void TeteNode::registerLinkCreate(){
    d_->getScene()->main_view()->link_creator()->registerClick(tete_->id());
  }

  void TeteNode::cancelLinkCreate(){
    d_->getScene()->main_view()->link_creator()->clear();
  }

  void TeteNode::focusOnUser(){
    //std::cerr << "Focus On User" << std::endl;
    focusOnLastUserTete();
  }

  void TeteNode::focusOnLastUserTete(){
    //std::cerr << "Focus on Last User Tete" << std::endl;
    // Fuck I fucked up. This is not a good way to structure a
    // program, the UsersScene is not a dumping ground for Tete data.
    // Oh well.
    d_->users_scene()->other_users_node()->focusOnLastNode(tete_->owner_id());
  }

  void TeteNode::playbackFromHere(){

  }


  NodeGradient* TeteNode::node_gradient(){
    return node_gradient_;
  }

}
