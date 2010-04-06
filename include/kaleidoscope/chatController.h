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
  class ChatLinkSystem;
  class ChatNode;
  class Link;

  class ChatController : public QObject {
    Q_OBJECT
  public:
    ChatController(Device*, QObject* parent = 0);
    ~ChatController();
    void init();

    GridsID default_chat_id();
    void set_default_chat_id(GridsID);
    Tete* getTeteFromID(GridsID);
    Link* getLinkFromID(GridsID);
    Chat* getChatFromID(GridsID);

    void addChat(Chat*);
    void addTete(Tete*);
    // Returns T if added, F if not
    bool addLink(Link*);
    // Deletes all tete nodes in this conversation from this point down
    void deleteTeteNodeTree(Tete* tete);


    void checkReframe();
    void zoomOut();
    void updateChatsRect();
    void fixBrokenLinks();

    std::vector<Chat*> chats();
    ChatLinkSystem* link_system();
    QRectF all_chats_rect();
    bool reframing();
    Tete* last_selected();

    unsigned int numLinks();

  protected:
    void timerEvent(QTimerEvent*);

  private:
    Device* d_;
    std::vector<Chat*> chats_;
    std::vector<Tete*> tetes_;
    std::vector<Link*> links_;
    std::vector<Link*> broken_links_;
    std::vector<ChatNode*> chat_nodes_;
    int chat_refresh_;
    GridsID default_chat_id_;
    QRectF all_chats_rect_;
    float zoom_out_speed_;
    float zoom_margin_;
    Tete* last_selected_;
    ChatLinkSystem* link_system_;
    bool reframing_;
    bool chat_reframing_;
  };
}

#endif // CHATCONTROLLER_H
