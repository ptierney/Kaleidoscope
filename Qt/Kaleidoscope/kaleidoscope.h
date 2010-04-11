#ifndef KALEIDOSCOPE_H
#define KALEIDOSCOPE_H

#include <QtGui/QMainWindow>


namespace Ui
{
  class Kaleidoscope;
}

namespace Kaleidoscope {
  class Device;

  class Kaleidoscope : public QMainWindow {
    Q_OBJECT

  public:
    Kaleidoscope(QApplication* ap, QWidget *parent = 0);
    ~Kaleidoscope();

    QSize sizeHint() const;
    void createActions();
    void createMenus();


  private slots:
    void init();
    void createUserInputWindow();
    void createAddUserWindow();
    void createNewChatWindow();

  private:
    Ui::Kaleidoscope *ui;
    Device* d;

    QAction* set_name_act_;
    QAction* register_username_;
    QAction* new_chat_;
    QMenu* start_menu_;
  };

} // end namespace Kaleidoscope

#endif // KALEIDOSCOPE_H
