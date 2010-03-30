#ifndef CHAT_H
#define CHAT_H

#include <vector>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Tete;
  class ChatNode;

  class Chat {
  public:
    Chat(Device*, GridsID);

    void addTete(Tete*);
    std::vector<Tete*> tetes();

    GridsID chat_id();
    void set_chat_id(GridsID);

    ChatNode* chat_node();
    void set_chat_node(ChatNode*);

  private:
    Device* d_;
    std::vector<Tete*> tetes_;
    GridsID chat_id_;
    ChatNode* chat_node_;
  };
}

#endif // CHAT_H
