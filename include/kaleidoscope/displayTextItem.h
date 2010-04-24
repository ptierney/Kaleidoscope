#ifndef DISPLAYTEXTITEM_H
#define DISPLAYTEXTITEM_H

// A simple item to display text, though that
// can allow the user to edit the text

#include <QGraphicsTextItem>
#include <QTime>
#include <QTextLayout>

namespace Kaleidoscope {
  class Device;
  class DisplayTextNode;

  class DisplayTextItem : public QGraphicsTextItem {
    Q_OBJECT

  public:
    DisplayTextItem(Device* d,
                    DisplayTextNode* node,
                    QGraphicsItem* parent = 0);
    void init();
    void makeActive();
    void makeNotActive();
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);
    void setText(std::string);
    void setActiveText(std::string);
    void appendText(std::string);
    void appendActiveText(std::string);
    bool initial_cursor_moved();
    bool moveCursorToEnd();

    void setPlainText(const QString& text);

  protected:
    void timerEvent(QTimerEvent *);
    void tryToMoveCursor();
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void keyPressEvent(QKeyEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  private:
    Device* d_;
    DisplayTextNode* node_;
    bool initial_cursor_moved_;
    int cursor_timer_id_;
    int key_timer_id_;

    QTime key_timer_;
    QTime last_key_press_;
    bool keys_unsent_;
    bool input_unsent_;
    int key_delay_;
    int input_time_;
    // When a text item looses focus (when it goes from being
    // an editable text box to a text display), this item
    // should be deleted, and replaced with a simple text
    // display with the same text.
    bool replace_me_;
    bool active_;
    bool text_updated_;
    QTextLayout* layout_;
  };

}

#endif // DISPLAYTEXTITEM_H
