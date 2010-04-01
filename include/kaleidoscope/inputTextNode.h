#ifndef INPUTTEXTNODE_H
#define INPUTTEXTNODE_H

#include <QTime>

#include <kaleidoscope/textNode.h>

namespace Kaleidoscope {
  class Device;

  class InputTextNode : public TextNode, public TeteInterface {

  public:
    InputTextNode(Device*, QGraphicsItem* parent = 0,
                  QGraphicsScene* scene = 0);
    void init();

    void placeNode();

  protected:
    void keyPressEvent(QKeyEvent* event);

  private:
    // The rate at which to send what the user has typed
    int key_delay_;
    // After this amount of inactivity, the input is registered.
    int input_time_;

    QTime key_timer_;
    QTime last_key_press_;
  };
}

#endif // INPUTTEXTNODE_H
