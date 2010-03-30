
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
    node_y_space_ = 15;
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

  void ChatNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    updateDrawRect();
  }

  void ChatNode::updateDrawRect(){
    updateAllTetesRect();
    draw_rect_ = all_tetes_rect_;
  }

  void ChatNode::updateAllTetesRect(){
    float min_x = all_tetes_rect_.topLeft().x();
    float min_y = all_tetes_rect_.topLeft().y();
    float max_x = all_tetes_rect_.bottomRight().x();
    float max_y = all_tetes_rect_.bottomRight().y();

    std::vector<Tete*> tetes = chat_->tetes();

    for(unsigned int i = 0u; i < tetes.size(); i++){
      QRectF local_bound = tetes[i]->tete_node()->boundingRect();
      QRectF bound = local_bound;
      bound.moveTo(tetes[i]->tete_node()->pos());
      // Translate more due to text
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

    all_tetes_rect_ = QRectF(QPointF(min_x, min_y),
                             QPointF(max_x, max_y)).normalized();
  }

  bool ChatNode::frameOn(){

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

    for(int i = 0; i < children.size(); i++){
      placeNodes(children[i]);
    }
  }

}
