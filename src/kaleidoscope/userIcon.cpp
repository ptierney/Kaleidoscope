
#include <iostream>

#include <QFontMetricsF>
#include <QTextLayout>
#include <QMenu>
#include <QTimer>
#include <QAction>
#include <QCursor>

#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/userIcon.h>
#include <kaleidoscope/usersScene.h>
#include <kaleidoscope/usersView.h>
#include <kaleidoscope/user.h>
#include <kaleidoscope/otherUsersNode.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope {

  UserIcon::UserIcon(Device* d, User* user, QGraphicsItem* parent) :
      QGraphicsObject(parent) {
    d_ = d;
    user_ = user;
    layout_ = NULL;
    menu_ = NULL;
    mouse_released_ = true;
  }

  UserIcon::~UserIcon(){
    delete user_;
    delete menu_;
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

    menu_ = new QMenu(tr(user_->name().c_str()));
    show_on_map_ = new QAction(tr("Show on map"), this);
    menu_->addAction(show_on_map_);
    connect(show_on_map_, SIGNAL(triggered()),
            this,
            SLOT(activateLastNode()));
    focus_on_ = new QAction(tr("Focus on"), this);
    menu_->addAction(focus_on_);
    connect(focus_on_, SIGNAL(triggered()),
            this,
            SLOT(focusOnLastNode()));

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

  void UserIcon::createMenuCallback(){
    if(mouse_released_)
      return;
    //std::cerr << "Creating menu" << std::endl;

    QPoint mouse_global = d_->getScene()->main_view()->cursor().pos();
    menu_->popup(mouse_global);
  }

  void UserIcon::mousePressEvent(QGraphicsSceneMouseEvent* /*event*/){
    mouse_released_ = false;
    //std::cerr << user_->name() << std::endl;
    activateLastNode();
    QTimer::singleShot(700, this, SLOT(createMenuCallback()));
  }

  void UserIcon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* /*event*/){
    focusOnLastNode();
  }

  void UserIcon::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/){
    mouse_released_ = true;
  }

  void UserIcon::activateLastNode(){
    d_->users_scene()->other_users_node()->activateLastNode(user_->id());
  }

  void UserIcon::focusOnLastNode(){
    d_->users_scene()->other_users_node()->focusOnLastNode(user_->id());
  }



}
