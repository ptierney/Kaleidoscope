#ifndef CHAT_H
#define CHAT_H

#include <vector>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Tete;
  class ChatNode;
  class Link;

  class Chat {
  public:
    Chat(Device*, GridsID);

    void addTete(Tete*);
    // Add tete and link up parent / child relationships
    void addTeteAsTree(Tete*);
    std::vector<Tete*> tetes();
    void addLink(Link*);
    std::vector<Link*> links();

    GridsID chat_id();
    void set_chat_id(GridsID);

    ChatNode* chat_node();
    void set_chat_node(ChatNode*);

  private:
    Device* d_;
    std::vector<Tete*> tetes_;
    std::vector<Link*> links_;
    GridsID chat_id_;
    ChatNode* chat_node_;
  };
}

#endif // CHAT_H
