
#include <string>
#include <iostream>

#include <QtGlobal>
#include <QString>
#include <QColor>

#include <kaleidoscope/user.h>

namespace Kaleidoscope {

  User::User(Device* d, GridsID id, QObject* parent) :
      QObject(parent) {
    d_ = d;
    id_ = id;
    QString name_index;
    name_index.setNum((qrand() % 10 + 1));
    name_ = std::string("John Doe ") + name_index.toStdString();
    color_ = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
    has_set_color_ = false;
    has_set_name_ = false;
  }

  std::string User::name(){
    return name_;
  }

  bool User::hasSetName(){
    return !name_.empty();
  }

  void User::set_name(std::string name){
    name_ = name;
    has_set_name_ = true;
  }

  GridsID User::id(){
    return id_;
  }

  void User::set_id(GridsID id){
    id_ = id;
  }

  void User::set_color(QColor user_color){
    //std::cerr << "Set color" << std::endl;
    color_ = user_color;
    has_set_color_ = true;
  }

  QColor User::color(){
    return color_;
  }

  bool User::has_set_name(){
    return has_set_name_;
  }

  bool User::has_set_color(){
    return has_set_color_;
  }



}
