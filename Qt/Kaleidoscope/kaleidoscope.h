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
    void createColorPickWindow();
    void setSpringType1();
    void setSpringType2();
    void setGraphicsType1();
    void setGraphicsType2();
    void springToggle();
    void gradientToggle();
    void zoomToggle();
    void printScene();

  private:
    Ui::Kaleidoscope *ui;
    Device* d;

    QAction* set_name_;
    QAction* set_color_;
    QAction* register_username_;
    QAction* new_chat_;
    QAction* print_;
    QMenu* start_menu_;
    QMenu* explore_menu_;
    QMenu* spring_menu_;
    QMenu* graphics_menu_;
    QMenu* file_menu_;
    QAction* spring_type_1_;
    QAction* spring_type_2_;
    QAction* graphics_type_1_;
    QAction* graphics_type_2_;
    QAction* spring_on_off_;
    QAction* gradient_on_off_;
    QAction* zoom_on_off_;
  };

} // end namespace Kaleidoscope

#endif // KALEIDOSCOPE_H
