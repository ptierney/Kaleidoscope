
#include <kaleidoscope/user.h>

namespace Kaleidoscope {

  User::User(Device* d, GridsID id){
    d_ = d;
    id_ = id;
    name_ = id;
  }

  std::string User::name(){
    return name_;
  }

  void User::set_name(std::string name){
    name_ = name;
  }

  GridsID User::id(){
    return id_;
  }

  void User::set_id(GridsID id){
    id_ = id;
  }

}
