
#include <string>

#include <QtGlobal>
#include <QString>

#include <kaleidoscope/user.h>

namespace Kaleidoscope {

  User::User(Device* d, GridsID id){
    d_ = d;
    id_ = id;
    QString name_index;
    name_index.setNum((qrand() % 10 + 1));
    name_ = std::string("John Doe ") + name_index.toStdString();
  }

  std::string User::name(){
    return name_;
  }

  bool User::hasSetName(){
    return !name_.empty();
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
