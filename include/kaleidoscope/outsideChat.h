#ifndef OUTSIDECHAT_H
#define OUTSIDECHAT_H

#include <iostream>

#include <kaleidoscope/define.h>

// A simple data structure to store screenname - protocol - chat ids

namespace Kaleidoscope {

  class OutsideChat {
  public:
    OutsideChat(std::string screen_name = "",
                std::string protocol = "",
                GridsID chat_id = "");

    std::string& screen_name();
    std::string& protocol();
    GridsID& chat_id();

  private:
    std::string screen_name_;
    std::string protocol_;
    GridsID chat_id_;

  };
}

#endif // OUTSIDECHAT_H
