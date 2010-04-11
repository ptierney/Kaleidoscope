
#include <QtGui>

#include <kaleidoscope/addUsernameBox.h>
#include <kaleidoscope/outsideChatController.h>

namespace Kaleidoscope {

  AddUsernameBox::AddUsernameBox(Device* d, QWidget* parent) :
      QWidget(parent) {
    d_ = d;
    parent_ = parent;

    username_text_box_ = new QLineEdit(this);
    password_text_box_ = new QLineEdit(this);
    password_text_box_->setEchoMode(QLineEdit::Password);
    protocol_combo_box_ = new QComboBox(this);
    protocol_combo_box_->addItem("AIM", "aim");
    protocol_combo_box_->addItem("Google Talk", "xmpp");
    button_box_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                       | QDialogButtonBox::Cancel);

    connect(password_text_box_, SIGNAL(returnPressed()),
            this, SLOT(done()));
    connect(button_box_, SIGNAL(accepted()),
            this, SLOT(done()));
    connect(button_box_, SIGNAL(rejected()),
            parent, SLOT(close()));

    QLabel* username_label = new QLabel(this);
    username_label->setText(tr("Screen Name:"));
    QLabel* password_label = new QLabel(this);
    password_label->setText(tr("Password:"));

    QVBoxLayout* main_layout = new QVBoxLayout;

    main_layout->addWidget(username_label);
    main_layout->addWidget(username_text_box_);
    main_layout->addWidget(password_label);
    main_layout->addWidget(password_text_box_);
    main_layout->addWidget(protocol_combo_box_);
    main_layout->addWidget(button_box_);

    setLayout(main_layout);

    username_text_box_->setFocus(Qt::PopupFocusReason);
  }

  void AddUsernameBox::done(){
    QString screen_name = username_text_box_->text();
    QString password = password_text_box_->text();
    QString protocol = protocol_combo_box_->currentText();
    if(protocol == tr("AIM")){
      protocol = tr("aim");
    } else if (protocol == tr("Google Talk")){
      protocol = tr("xmpp");
    }

    if(screen_name.toStdString().empty() ||
       password.toStdString().empty() ){
      parent_->close();
      return;
    }

    OutsideChatController::requestAddAccount(d_, protocol.toStdString(),
                                             screen_name.toStdString(),
                                             password.toStdString() );

    parent_->close();
  }






}
