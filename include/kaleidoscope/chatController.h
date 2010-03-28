#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <vector>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Chat;
  class Tete;

  class ChatController {
  public:
    ChatController(Device*);
    ~ChatController();
    void init();

    GridsID default_chat_id();
    void set_default_chat_id(GridsID);

    void addChat(Chat*);
    void addTete(Tete*);

    void checkReframe();

  private:
    Device* d_;
    std::vector<Chat*> chats_;
    std::vector<Tete*> tetes_;
    int chat_refresh_;
    GridsID default_chat_id_;
  };
}

#endif // CHATCONTROLLER_H
