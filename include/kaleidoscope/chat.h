#ifndef CHAT_H
#define CHAT_H

#include <vector>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Tete;

  class Chat {
  public:
    Chat(Device*, GridsID);

    void addTete(Tete*);
    std::vector<Tete*> tetes();

    GridsID chat_id();
    void set_chat_id(GridsID);

  private:
    Device* d_;
    std::vector<Tete*> tetes_;
    GridsID chat_id_;
  };
}

#endif // CHAT_H
