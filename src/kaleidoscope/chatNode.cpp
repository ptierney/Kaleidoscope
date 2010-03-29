
#include <kaleidoscope/chatNode.h>
#include <kaleidoscope/chatLinkSystem.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/teteNode.h>

namespace Kaleidoscope {

  ChatNode::ChatNode(Device* d, Chat* chat,
                     QGraphicsItem* parent, QGraphicsScene* scene) :
    RespondNode(d, parent, scene) {
    chat_ = chat;
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

}
