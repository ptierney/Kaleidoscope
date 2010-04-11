#ifndef OUTSIDECHATCONTROLLER_H
#define OUTSIDECHATCONTROLLER_H

#include <QObject>

#include <kaleidoscope/define.h>

namespace Grids{
  class Event;
}

namespace Kaleidoscope {
  class Device;

  class OutsideChatController : public QObject {
    Q_OBJECT
  public:
    OutsideChatController(Device* d, QObject* parent = 0);

    static GridsID requestAddAccount(Device* d, std::string protocol,
                                     std::string screen_name,
                                     std::string password);

    // This sends a message to someone on an outside chat protocol
    static GridsID requestOutsideChat(Device* d,
                                      std::string chat,
                                      std::string protocol,
                                      std::string your_screen_name,
                                      std::string other_screen_name,
                                      std::string message);

    // This create a node from a message from an outside chat protocol
    static void gridsCreateOutsideChat(Device* d, Grids::Event* event);
  private:
    Device* d_;

  };
}

#endif // OUTSIDECHATCONTROLLER_H
