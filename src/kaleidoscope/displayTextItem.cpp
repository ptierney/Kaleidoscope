
#include <QTextCursor>
#include <QTimerEvent>

#include <kaleidoscope/displayTextItem.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/displayTextNode.h>

namespace Kaleidoscope {

  DisplayTextItem::DisplayTextItem(Device* d,
                                   DisplayTextNode* node,
                                   QGraphicsItem* parent) :
  QGraphicsTextItem(parent) {
    d_ = d;
    node_ = node;
    initial_cursor_moved_ = false;
    // Send update messages at this rate
    key_delay_ = 250;
    // After this time focus is lost.
    input_time_ = 15000;
    keys_unsent_ = false;
    input_unsent_ = false;
  }

  void DisplayTextItem::init(){
    last_key_press_.start();
    key_timer_id_ = startTimer((int)(key_delay_ * 0.8));
  }

  void DisplayTextItem::makeActive(){
    if (textInteractionFlags() == Qt::NoTextInteraction){
      setFlag(QGraphicsItem::ItemIsFocusable);
      setFlag(QGraphicsItem::ItemIsSelectable);
      setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    setFocus(Qt::OtherFocusReason);
  }

  void DisplayTextItem::makeNotActive(){
    node_->centerTextItem();
    setTextInteractionFlags(Qt::NoTextInteraction);
    node_->tete()->updateText(toPlainText().toStdString());
  }


  void DisplayTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    makeActive();
    //text_item_->mouseDoubleClickEvent(event);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
  }

  void DisplayTextItem::focusInEvent(QFocusEvent* /*event*/) {
    //setTextInteractionFlags(Qt::TextEditorInteraction);
    initial_cursor_moved_ = textCursor().movePosition(QTextCursor::End);

    if(!initial_cursor_moved_){
      cursor_timer_id_ = startTimer(50);
    }
  }

  void DisplayTextItem::focusOutEvent(QFocusEvent *event) {
    makeNotActive();
    QGraphicsTextItem::focusOutEvent(event);
  }

  void DisplayTextItem::appendText(std::string text){
    setPlainText(toPlainText() + tr(text.c_str()));
  }

  void DisplayTextItem::appendActiveText(std::string text){
    appendText(text);
    QTextCursor temp_cursor = textCursor();
    initial_cursor_moved_ = temp_cursor.movePosition(QTextCursor::End);
    setTextCursor( temp_cursor );
  }

  void DisplayTextItem::setText(std::string text){
    setPlainText(tr(text.c_str()));
  }

  void DisplayTextItem::setActiveText(std::string text){
    setText(text);
    QTextCursor temp_cursor = textCursor();
    initial_cursor_moved_ = temp_cursor.movePosition(QTextCursor::End);
    setTextCursor( temp_cursor );
  }

  bool DisplayTextItem::initial_cursor_moved(){
    return initial_cursor_moved_;
  }

  void DisplayTextItem::tryToMoveCursor(){
    if(moveCursorToEnd()){
      killTimer(cursor_timer_id_);
    }
  }

  bool DisplayTextItem::moveCursorToEnd(){
    QTextCursor temp_cursor = textCursor();
    initial_cursor_moved_ = temp_cursor.movePosition(QTextCursor::End);
    setTextCursor( temp_cursor );
    return initial_cursor_moved_;
  }

  /* This doesn't seem to be working. */
  QVariant DisplayTextItem::itemChange(GraphicsItemChange change,
                                     const QVariant &value) {
    if (change == QGraphicsItem::ItemCursorChange) {
      if(hasFocus()){
        node_->tete()->updateText(toPlainText().toStdString());
      }
    }

    return QGraphicsItem::itemChange(change, value);
  }

  void DisplayTextItem::keyPressEvent(QKeyEvent* event){
    // This updates the text toPlainText() returns
    QGraphicsTextItem::keyPressEvent(event);

    if(!hasFocus())
      return;

    if(keys_unsent_ == false){
      keys_unsent_ = true;
      key_timer_.start();
    }

    input_unsent_ = true;
    last_key_press_.start();
  }

  void DisplayTextItem::timerEvent(QTimerEvent* event){
    if(event->timerId() == cursor_timer_id_){
      tryToMoveCursor();
      return;
    }

    if(!hasFocus())
      return;

    if(keys_unsent_ && key_timer_.elapsed() > key_delay_){
      keys_unsent_ = false;
      node_->tete()->updateText(toPlainText().toStdString());
    }

    if(last_key_press_.elapsed() > input_time_){
      makeNotActive();
    }
  }

}
