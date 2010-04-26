
#include <QFontMetricsF>
#include <QTextLayout>

#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/usersView.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UserIcon::UserIcon(Device* d, User* user, QGraphicsItem* parent) :
      QGraphicsObject(parent) {
    d_ = d;
    user_ = user;
    layout_ = NULL;
  }

  UserIcon::~UserIcon(){
    delete user_;
  }

  void UserIcon::init(){
    QString text = tr(user_->name().c_str());
    layout_ = new QTextLayout(text,
                              d_->users_scene()->font());
    QFontMetricsF fm(d_->users_scene()->font());
    layout_->beginLayout();
    QTextLine line = layout_->createLine();
    //line.setNumColumns(1);
    QSizeF text_size_ = fm.boundingRect(text).size();
    // TODO: Figure out what these numbers are all about, why width/4, height/4
    //line.setPosition(QPointF(s.width()/4.0,
                            // -s.height()/4.0));
    line.setPosition(QPointF());

    layout_->endLayout();
    layout_->setCacheEnabled(true);
    updateDrawRect();
  }

  QRectF UserIcon::boundingRect() const {
    qreal adjust = 2;
    return draw_rect_.adjusted(-adjust, -adjust, adjust, adjust).normalized();
  }

  QPainterPath UserIcon::shape() const {
    QPainterPath path;
    path.addRect(draw_rect_);
    return path;
  }

  void UserIcon::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {
    if(layout_)
      layout_->draw(painter, QPointF());

    painter->setPen(QPen(user_->color()));
    painter->setBrush(QBrush(user_->color()));
    QRectF color_box = QRectF( -draw_rect_.height() * 2.0,
                               0.0,
                               draw_rect_.height(),
                               draw_rect_.height());
    painter->drawRect(color_box);
  }

  void UserIcon::updateDrawRect(){
    draw_rect_ = layout_->boundingRect();
  }

  User* UserIcon::user(){
    return user_;
  }

  void UserIcon::set_user(User* user){
    user_ = user;
  }

}
