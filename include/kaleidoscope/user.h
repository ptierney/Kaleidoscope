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
    void set_id(GridsID);
    std::string name();
    void set_name(std::string); 
    bool hasSetName();
    QColor color();

  public slots:
    void set_color(QColor);

  private:
    Device* d_;
    GridsID id_;
    std::string name_;
    QColor color_;
  };
}


#endif // USER_H
