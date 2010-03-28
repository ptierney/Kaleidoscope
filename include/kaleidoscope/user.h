#ifndef USER_H
#define USER_H

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;

  class User {
  public:
    User(Device*, GridsID);

    GridsID id();
    void set_id(GridsID);
    std::string name();
    void set_name(std::string);

  private:
    Device* d_;
    GridsID id_;
    std::string name_;
  };
}


#endif // USER_H
