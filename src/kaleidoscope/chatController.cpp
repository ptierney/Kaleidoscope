
#include <algorithm>

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
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  ChatController::ChatController(Device* d, QObject* parent) :
    QObject(parent) {
    d_ = d;
    chat_refresh_ = 50;
    zoom_out_speed_ = 0.04;
    zoom_margin_ = 75;
    last_selected_ = NULL;
  }

  ChatController::~ChatController(){
    Chat* temp_chat;
    for(std::vector<Chat*>::iterator i = chats_.begin(); i != chats_.end(); i++){
      temp_chat = *i;
      delete temp_chat;
    }

    delete link_system_;
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

  void ChatController::timerEvent(QTimerEvent* /*event*/){
    checkReframe();
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

      // Should this be a const_iterator?
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

    // Update the node placements
    //chat->chat_node()->placeNodes();
    updateChatsRect();
  }

  bool ChatController::addLink(Link* link){
    // If the link is set up already, don't do anything special
    if(link->node_1() && link->node_2()){
      links_.push_back(link);
      return true;
    }

    GridsID node_1_id = link->node_1_id();
    GridsID node_2_id = link->node_2_id();

    Tete* node_1 = getTeteFromID(node_1_id);
    Tete* node_2 = getTeteFromID(node_2_id);

    if(node_1 == NULL || node_2 == NULL)
      return false;

    link->set_node_1(node_1);
    link->set_node_2(node_2);

    node_1->addLink(link);
    node_2->addLink(link);

    link_system_->set_running(true);

    return true;
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
    bool found = false;
    unsigned int i;
    for(i = 0u; i < tetes_.size(); i++){
      if(tetes_[i]->getID() == id){
        found = true;
        break;
      }
    }

    if(found)
      return tetes_[i];

    return NULL;
  }

  Link* ChatController::getLinkFromID(GridsID id){
    bool found = false;
    unsigned int i;
    for(i = 0u; i < links_.size(); i++){
      if(links_[i]->getID() == id){
        found = true;
        break;
      }
    }

    if(found)
      return links_[i];

    return NULL;
  }

  void ChatController::updateChatsRect(){
    if(tetes_.empty()){
      all_chats_rect_ = QRectF(-1, -1, 2, 2);
      return;
    }

    TeteNode* first_node = NULL;

    for(unsigned int i = 0u; first_node == NULL; i++){
      first_node = tetes_[i]->tete_node();
    }

    QRectF local_bound = first_node->boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(first_node->pos());
    // Translate more due to text
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    float min_x = bound.topLeft().x();
    float min_y = bound.topLeft().y();
    float max_x = bound.bottomRight().x();
    float max_y = bound.bottomRight().y();

    for(unsigned int i = 0u; i < tetes_.size(); i++){
      QRectF local_bound = tetes_[i]->tete_node()->boundingRect();
      QRectF bound = local_bound;
      bound.moveTo(tetes_[i]->tete_node()->pos());
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

    all_chats_rect_ = QRectF(QPointF(min_x, min_y),
                             QPointF(max_x, max_y)).normalized();
  }

  void ChatController::checkReframe(){
    reframing_ = false;

    for(unsigned int i = 0u; i < tetes_.size(); i++){
      if( tetes_[i] && tetes_[i]->tete_node() ){
        if( tetes_[i]->tete_node()->selected() ){
          reframing_ = true;
          last_selected_ = tetes_[i];
          tetes_[i]->tete_node()->frameOn();
          }
      }
    }

    if(reframing_)
      return;

    if(last_selected_ && last_selected_->tete_node()){
      if(last_selected_->tete_node()->frame_selected())
        return;
    }

    // Zoom out
    zoomOut();
  }

  void ChatController::zoomOut(){
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

  std::vector<Chat*> ChatController::chats(){
    return chats_;
  }

  QRectF ChatController::all_chats_rect(){
    return all_chats_rect_;
  }

  bool ChatController::reframing(){
    return reframing_;
  }

} // end namespace
