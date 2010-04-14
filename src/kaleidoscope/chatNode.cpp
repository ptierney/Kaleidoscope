#include <iostream>

#include <QPainter>

#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/teteNode.h>
#include <kaleidoscope/displayTextNode.h>

namespace Kaleidoscope {

  ChatNode::ChatNode(Device* d, Chat* chat,
                     QGraphicsItem* parent, QGraphicsScene* scene) :
    RespondNode(d, parent, scene) {
    chat_ = chat;
    node_y_space_ = 100;
  }

  void ChatNode::init(){
    all_tetes_rect_ = QRectF(-10, -10, 20, 20);
    draw_rect_ = all_tetes_rect_;
  }

  QRectF ChatNode::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath ChatNode::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void ChatNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/){

    updateDrawRect();

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    painter->drawRect(draw_rect_);
  }

  void ChatNode::updateDrawRect(){
    updateAllTetesRect();
    draw_rect_ = all_tetes_rect_;
  }

  void ChatNode::updateAllTetesRect(){
    std::vector<Tete*> tetes = chat_->tetes();

    if(tetes.empty()){
      all_tetes_rect_ = QRectF(-1, -1, 2, 2);
      return;
    }

    TeteNode* first_node = NULL;
    // Find the first non-null node
    for(unsigned int i = 0u; first_node == NULL; i++){
      first_node = tetes[i]->tete_node();
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

    for(std::vector<Tete*>::const_iterator it = tetes.begin(); it != tetes.end(); ++it){
      addTeteToMinMax(*it, &min_x, &min_y, &max_x, &max_y);
    }

    all_tetes_rect_ = QRectF(QPointF(min_x, min_y),
                             QPointF(max_x, max_y)).normalized();
  }

  void ChatNode::addTeteToMinMax(Tete* tete,
                                 float* min_x, float* min_y,
                                 float* max_x, float* max_y) {
    //return;
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    QRectF local_bound = tete->tete_node()->boundingRect();
    QRectF bound = local_bound;
    bound.moveTo(tete->tete_node()->pos());
    bound.translate(-local_bound.width()/2,-local_bound.height()/2);

    //std::cerr << bound.topLeft().y() << " " << *min_y << std::endl;

    if(bound.topLeft().x() < *min_x)
      *min_x = bound.topLeft().x();
    if(bound.topLeft().y() < *min_y)
      *min_y = bound.topLeft().y();
    if(bound.bottomRight().x() > *max_x)
      *max_x = bound.bottomRight().x();
    if(bound.bottomRight().y() > *max_y)
      *max_y = bound.bottomRight().y();
  }

  bool ChatNode::frameOn(){
    return false;
  }

  void ChatNode::buildNodeTree(){
    if(chat_->tetes().empty())
      return;
    else
      buildNodeTree(chat_->tetes().front());
  }

  void ChatNode::buildNodeTree(Tete* tete){
    // Find the position of this Tete in the list
    std::vector<Tete*> tetes = chat_->tetes();
    unsigned int index;
    for(index = 0u; index < tetes.size(); index++){
      if(tetes[index] == tete)
        break;
    }

    // Return if it didn't find the Tete
    if(index == tetes.size())
      return;

    // TODO: this only works for one type of Tete,
    // it should work for any Tete type
    DisplayTextNode* display_node;
    for(; index < tetes.size(); index++){
      if(tetes[index]->tete_node())
        continue;
      display_node = new DisplayTextNode(d_, tetes[index]);
      display_node->init();
      tetes[index]->set_tete_node(display_node);
    }
  }

  void ChatNode::placeNodes(){
    if(chat_->tetes().empty())
      return;
    else
      placeNodes(chat_->tetes().front());
  }

  void ChatNode::placeNodes(Tete* tete){
    if(tete == NULL)
      return;
    if(tete->tete_node() == NULL)
      return;

    std::vector<Tete*> children = tete->children();

    if(children.empty())
      return;

    //TODO: Fix placement gap
    TeteNode* tete_node = tete->tete_node();
    float x, y;

    for(unsigned int i = 0u; i < children.size(); i++){
      if(children[i]->tete_node() == NULL)
        continue;
      x = tete_node->pos().x() +
          tete_node->boundingRect().width() / 2.0 +
          node_y_space_ +
          children[i]->tete_node()->boundingRect().width() / 2.0;

      float total_y_height = children[i]->tete_node()->boundingRect().height() *
                             (children.size()-1) +
                             node_y_space_ *
                             (children.size()-1);

      y = tete_node->pos().y() +
          - total_y_height / 2.0 +
          (children[i]->tete_node()->boundingRect().height() +
          node_y_space_) * (float)i;

      children[i]->tete_node()->setPos(x, y);
    }

    for(unsigned int i = 0u; i < children.size(); i++){
      placeNodes(children[i]);
    }
  }

  Vec3D ChatNode::velocity(){
    return velocity_;
  }

  void ChatNode::set_velocity(Vec3D velocity){
    velocity_ = velocity;
  }

  void ChatNode::addVelocity(Vec3D addition){
    velocity_ += addition;
  }

  void ChatNode::updatePosition(){
    setPos(x() + velocity_.X, y() + velocity_.Y);
  }

}
