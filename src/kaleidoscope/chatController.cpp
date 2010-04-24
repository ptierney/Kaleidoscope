
#include <algorithm>
#include <iostream>

#include <kaleidoscope/chatController.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/teteNode.h>
#include <grids/objectController.h>
#include <kaleidoscope/noticeWindow.h>
#include <grids/utility.h>
#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/link.h>
#include <kaleidoscope/linkNode.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatController::ChatController(Device* d, QObject* parent) :
    QObject(parent) {
    d_ = d;
    chat_refresh_ = 50;
    zoom_out_speed_ = 0.04;
    zoom_margin_ = 40;
    last_selected_ = NULL;
    reframing_ = false;
    chat_reframing_ = false;
    zooming_ = true;
    start_zooming_timer_ = 0;
    zoom_delay_ = 3 * 1000;
    spring_toggle_ = true;
    gradient_toggle_ = true;
    zooming_toggle_ = true;
    zoom_out_needed_ = false;
    // The closer to 1, the more likely
    zoom_threshold_ = 1.001;
  }

  ChatController::~ChatController(){
    Chat* temp_chat;
    for(std::vector<Chat*>::iterator it = chats_.begin(); it != chats_.end(); ++it){
      temp_chat = *it;
      delete temp_chat; // Each chat deletes all the Tete*s it knows
      // TODO: erase element of vector?
    }

    for(std::vector<Link*>::iterator it = links_.begin(); it != links_.end(); ++it){
      delete *it;
    }

    delete link_system_;

    // Remember that all Tete / Link / Chat Nodes are deleted by Qt if they are
    // childs of a Qt object (if they are displayed).
  }

  void ChatController::init(){
    default_chat_id_ = d_->getGridsUtility()->getNewUUID();

    //int main_width = d_->main_window->centralWidget()->size().width();
    //int main_height = d_->main_window->centralWidget()->size().height();
    /*all_chats_rect_ = QRectF(-main_width/2, -main_height/2,
      main_width, main_height);*/
    all_chats_rect_ = QRectF(-10, -10, 20, 20);
    all_chats_rect_ = all_chats_rect_.normalized();

    startTimer(chat_refresh_);

    link_system_ = new ChatLinkSystem(d_);
  }

  void ChatController::timerEvent(QTimerEvent* event){
    //return;

    // Check reframe blocks
    checkReframe();

    if(event->timerId() == start_zooming_timer_){
      setStartZooming();
      killTimer(start_zooming_timer_);
    }
  }

  void ChatController::addChat(Chat* chat){
    chats_.push_back(chat);
  }

  void ChatController::addTete(Tete* tete){
    // Extract Chat
    Chat* chat = tete->chat();

    // Check if Chat is known
    // If not, create a new chat
    if(chat == NULL){
      // This should be the only place I make a new Chat
      GridsID chat_id = tete->chat_id();

      for(std::vector<Chat*>::const_iterator i = chats_.begin(); i != chats_.end(); i++){
        if((*i)->chat_id() == chat_id)
          chat = (*i);
      }

      if(chat == NULL){
        chat = new Chat(d_, chat_id);
        chats_.push_back(chat);
        ChatNode* chat_node = new ChatNode(d_, chat);
        chat_node->init();
        chat->set_chat_node(chat_node);
        chat_nodes_.push_back(chat_node);
        d_->getScene()->addItem(chat_node);
      }

      tete->set_chat(chat);
    }

    chat->addTeteAsTree(tete);
    tetes_.push_back(tete);

    fixBrokenLinks();

    // Update the node placements
    //chat->chat_node()->placeNodes();
    updateChatsRect();
    zoom_out_needed_ = true;

    if(tete->owner_id() == d_->user()->id())
      last_selected_ = tete;
  }

  bool ChatController::addLink(Link* link){
    if(link == NULL){
      //std::cerr << link << "Returning" << std::endl;
      return false;
    }

    // If the link is set up already, don't do anything special
    if(link->node_1() && link->node_2()){
      links_.push_back(link);
      return true;
    }

    GridsID node_1_id = link->node_1_id();
    GridsID node_2_id = link->node_2_id();

    Tete* node_1 = getTeteFromID(node_1_id);
    Tete* node_2 = getTeteFromID(node_2_id);

    if(node_1 == NULL || node_2 == NULL){
      std::vector<Link*>::const_iterator it;
      it = std::find(broken_links_.begin(),
                     broken_links_.end(),
                     link);
      if(it == broken_links_.end())
        broken_links_.push_back(link);

      return false;
    }

    link->set_node_1(node_1);
    link->set_node_2(node_2);

    node_1->addLink(link);
    node_2->addLink(link);

    //link_system_->set_running(true);
    link_system_->setChatRunning(node_1->chat_id());
    link_system_->setChatRunning(node_2->chat_id());

    return true;
  }

  void ChatController::fixBrokenLinks(){
    //std::cerr << broken_links_.size() << std::endl;

    for(std::vector<Link*>::iterator it = broken_links_.begin(); it != broken_links_.end(); ){
      if(addLink(*it)){
        it = broken_links_.erase(it);
      } else {
        ++it;
      }
    }
  }

  void ChatController::deleteTeteNodeTree(Tete *tete){
    std::vector<Tete*> children = tete->children();
    if(tete->tete_node()){
      delete tete->tete_node();
      tete->set_tete_node(NULL);
    }

    for(unsigned int i = 0u; i < children.size(); i++){
      deleteTeteNodeTree(children[i]);
    }
  }

  Tete* ChatController::getTeteFromID(GridsID id){
    Tete* found_tete = NULL;
    for(std::vector<Tete*>::const_iterator it = tetes_.begin(); it != tetes_.end(); ++it){
      if((*it)->id() == id){
        found_tete = *it;
        break;
      }
    }

    return found_tete;
  }

  Link* ChatController::getLinkFromID(GridsID id){
    Link* found_link = NULL;

    for(std::vector<Link*>::const_iterator it = links_.begin(); it != links_.end(); ++it){
      if((*it)->id() == id){
        found_link = *it;
        break;
      }
    }

    return found_link;
  }

  Chat* ChatController::getChatFromID(GridsID id){
    Chat* found_chat = NULL;
    for(std::vector<Chat*>::const_iterator it = chats_.begin(); it != chats_.end(); ++it) {
      if((*it)->id() == id){
        found_chat = *it;
        break;
      }
    }

    return found_chat;
  }

  void ChatController::updateChatsRect(){
    if(tetes_.empty()){
      all_chats_rect_ = QRectF(-1, -1, 2, 2);
      return;
    }

    TeteNode* first_node = NULL;
    // Find the first Tete with a TeteNode
    for(std::vector<Tete*>::const_iterator it = tetes_.begin(); first_node == NULL; ++it){
      first_node = (*it)->tete_node();
    }

    QRectF local_bound = first_node->boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(first_node->pos());
    // Translate more due to text
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    float start_min_x, start_min_y, start_max_x, start_max_y;
    float min_x = start_min_x = bound.topLeft().x();
    float min_y = start_min_y = bound.topLeft().y();
    float max_x = start_max_x = bound.bottomRight().x();
    float max_y = start_max_y = bound.bottomRight().y();

    for(std::vector<Tete*>::iterator it = tetes_.begin(); it != tetes_.end(); ++it){
      addTeteToMinMax(*it, min_x, min_y, max_x, max_y);
    }

    if(min_x == start_min_x)
      min_x -= (max_x - min_x);
    if(min_y == start_min_y)
      min_y -= (max_y - min_y);
    if(max_x == start_max_x)
      max_x += (max_x - min_x);
    if(max_y == start_max_y)
      max_y += (max_y - min_y);

    all_chats_rect_ = QRectF(QPointF(min_x, min_y),
                             QPointF(max_x, max_y)).normalized();
  }

  void ChatController::addTeteToMinMax(Tete* tete,
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

  void ChatController::checkReframe(){
    if(!zooming_)
      return;

    // Don't check if there's no tetes, it just messes up the scale for when tetes are actually added.
    if(tetes_.empty())
      return;

    if(zooming_toggle_ == false)
      return;

    reframing_ = false;
    //bool reframe_exception_ = false;

    for(std::vector<Tete*>::iterator it = tetes_.begin(); it != tetes_.end(); ++it){
      if( (*it) && (*it)->tete_node() ){
        if( (*it)->tete_node()->selected() ){
          reframing_ = true;
          last_selected_ = (*it);
          (*it)->tete_node()->frameOn();
          zoom_out_needed_ = true;
          }
        //std::cerr << "Checking" << std::endl;
        // This is for increasing the size of the fonts
        //(*it)->tete_node()->updateGeometry();
      }
    }

    if(reframing_)
      return;

    if(last_selected_ && last_selected_->tete_node()){
      if(last_selected_->tete_node()->frame_selected()){
        chat_reframing_ = true;
        return;
      }
    }
    chat_reframing_ = false;

    updateChatsRect();

    //return;

    // Zoom out locks
    if(zoom_out_needed_)
      zoomOut();
  }

  void ChatController::zoomOut(){
    //std::cerr << "zoom out" << qrand() << std::endl;
    View2D* view = d_->getScene()->main_view();

    int total_object_width = all_chats_rect_.width();
    int total_object_height = all_chats_rect_.height();

    float window_width = d_->main_window->centralWidget()->size().width();
    float window_height = d_->main_window->centralWidget()->size().height();

    float display_width = window_width - zoom_margin_*2.0;
    float display_height = window_height - zoom_margin_*2.0;

    float view_scale_width = display_width / total_object_width;
    float view_scale_height = display_height / total_object_height;

    float view_scale = view_scale_width <= view_scale_height ?
      view_scale_width :
      view_scale_height;

    QMatrix current_matrix = view->matrix();
    // Note: m11 and m22 hold the horizontal and vertical scale.
    // They should be the same.
    float current_scale = current_matrix.m11();

    float new_scale = current_scale + (view_scale - current_scale) * zoom_out_speed_;
   
    if(current_scale / new_scale < zoom_threshold_){
      //std::cerr << "Zoom out : " << current_scale / new_scale << std::endl;
      zoom_out_needed_ = false;
    }

    QMatrix temp_matrix;
    temp_matrix.scale(new_scale, new_scale);
    view->setMatrix(temp_matrix);

    /* It would be awesome if I could get this to work
    QPointF cursor_view = view->mapFromGlobal(QPoint( d_->main_window->cursor().pos() ));

    float current_center_x = d_->getScene()->width() / 2.0;
    float current_center_y = d_->getScene()->height() / 2.0;
    QPointF center_point = view->mapFromScene(current_center_x, current_center_y);

    float trans_dist_x = (cursor_view.x() - center_point.x()) * zoom_out_speed_/10.0;
    float trans_dist_y = (cursor_view.y() - center_point.y()) * zoom_out_speed_/10.0;
    */

    if(last_selected_ && last_selected_->tete_node()){
      //d_->getNoticeWindow()->write(7, "last selceted");
      view->centerOn(last_selected_->tete_node()->pos());
    }
    else
      view->centerOn(all_chats_rect_.center());

    /*
      d_->getScene()->main_view()->centerOn(center_point.x() + trans_dist_x,
      center_point.y() + trans_dist_y);
    */
    //d_->getScene()->main_view()->ensureVisible(all_chats_rect_, zoom_margin_, zoom_margin_);
  }

  GridsID ChatController::default_chat_id(){
    return default_chat_id_;
  }

  void ChatController::set_default_chat_id(GridsID default_chat_id){
    default_chat_id_ = default_chat_id;
  }

  const std::vector<Chat*>& ChatController::chats(){
    return chats_;
  }

  QRectF ChatController::all_chats_rect(){
    return all_chats_rect_;
  }

  bool ChatController::reframing(){
    return reframing_;
  }

  Tete* ChatController::last_selected(){
    return last_selected_;
  }

  ChatLinkSystem* ChatController::link_system(){
    return link_system_;
  }

  void ChatController::startZooming(){
    //start_zooming_timer_ = startTimer(zoom_delay_);
    //zooming_ = true;
    setStartZooming();
  }

  void ChatController::stopZooming(){
    zooming_ = false;
  }

  void ChatController::toggleZooming(){
    zooming_toggle_ = !zooming_toggle_;
  }

  void ChatController::toggleGradient(){
    gradient_toggle_ = !gradient_toggle_;\
  }

  void ChatController::toggleSpring(){
    spring_toggle_ = !spring_toggle_;
  }

  bool ChatController::spring_toggle(){
    return spring_toggle_;
  }

  bool ChatController::gradient_toggle(){
    return gradient_toggle_;
  }

  bool ChatController::zooming_toggle(){
    return zooming_toggle_;
  }

  void ChatController::setStartZooming(){
    zooming_ = true;
    // Update the last_selected_ to the closest node to the cursor
    last_selected_ = findClosestTete();
  }

  Tete* ChatController::findClosestTete(){
    QPointF mouse_pos = d_->getScene()->main_view()->cursor().pos();
    QPointF node_global_pos;
    Tete* closest_node = NULL;
    float closest_distance;
    float temp_distance;

    for(std::vector<Tete*>::const_iterator it = tetes_.begin(); it != tetes_.end(); ++it){
      if((*it)->tete_node() == NULL)
        continue;

      node_global_pos = (*it)->tete_node()->getGlobalPosition();

      if(closest_node == NULL){
        closest_distance = QLineF(mouse_pos, node_global_pos).length();
        closest_node = (*it);
        continue;
      }
      temp_distance = QLineF(mouse_pos, node_global_pos).length();
      // Compare the distance
      if(temp_distance < closest_distance){
        closest_distance = temp_distance;
        closest_node = (*it);
      }
    }

    return closest_node;
  }

} // end namespace
