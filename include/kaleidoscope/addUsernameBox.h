#ifndef ADDUSERNAMEBOX_H
#define ADDUSERNAMEBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>

namespace Kaleidoscope {
  class Device;

  class AddUsernameBox : public QWidget {
    Q_OBJECT
  public:
    AddUsernameBox(Device* d, QWidget* parent = 0);

  public slots:
    void done();

  private:
    Device* d_;
    QLineEdit* username_text_box_;
    QLineEdit* password_text_box_;
    QComboBox* protocol_combo_box_;
    QDialogButtonBox* button_box_;
    QWidget* parent_;

  };

}

#endif // ADDUSERNAMEBOX_H
