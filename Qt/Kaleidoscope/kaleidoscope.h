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
    void setSpringType1();
    void setSpringType2();
    void setGraphicsType1();
    void setGraphicsType2();

  private:
    Ui::Kaleidoscope *ui;
    Device* d;

    QAction* set_name_act_;
    QAction* register_username_;
    QAction* new_chat_;
    QMenu* start_menu_;
    QMenu* explore_menu_;
    QMenu* spring_menu_;
    QMenu* graphics_menu_;
    QAction* spring_type_1_;
    QAction* spring_type_2_;
    QAction* graphics_type_1_;
    QAction* graphics_type_2_;
  };

} // end namespace Kaleidoscope

#endif // KALEIDOSCOPE_H
