#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <vector>

#include <QObject>
#include <QGraphicsObject>

#include <kaleidoscope/define.h>

namespace Kaleidoscope {
  class Device;
  class Chat;
  class Tete;

  class ChatController : public QObject {
    Q_OBJECT
  public:
    ChatController(Device*, QObject* parent = 0);
    ~ChatController();
    void init();

    GridsID default_chat_id();
    void set_default_chat_id(GridsID);

    void addChat(Chat*);
    void addTete(Tete*);

    void checkReframe();
    void zoomOut();
    void updateChatsRect();

  protected:
    void timerEvent(QTimerEvent*);

  private:
    Device* d_;
    std::vector<Chat*> chats_;
    std::vector<Tete*> tetes_;
    int chat_refresh_;
    GridsID default_chat_id_;
    QRectF all_chats_rect_;
    float zoom_out_speed_;
    float zoom_margin_;
    Tete* last_selected_;
  };
}

#endif // CHATCONTROLLER_H
