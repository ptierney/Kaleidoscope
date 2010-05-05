#ifndef USER_H
#define USER_H

#include <QObject>
#include <QColor>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;

  class User : public QObject {
    Q_OBJECT

  public:
    User(Device*, GridsID, QObject* parent = 0);

    GridsID id();
    std::string name();
    void set_name(std::string); 
    void set_id(GridsID);
    bool hasSetName();
    QColor color();

    bool has_set_name();
    bool has_set_color();

  public slots:
    void set_color(QColor);

  private:
    Device* d_;
    GridsID id_;
    std::string name_;
    QColor color_;
    bool has_set_color_;
    bool has_set_name_;
  };
}


#endif // USER_H
