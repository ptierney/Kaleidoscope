#ifndef OUTSIDECHATCONTROLLER_H
#define OUTSIDECHATCONTROLLER_H

#include <map>
#include <string>
#include <QObject>

#include <kaleidoscope/define.h>

namespace Grids{
  class Event;
}

namespace Kaleidoscope {
  class Device;
  class Chat;

  // This map hold chats, indexed [protocol][screen_name]
  //typedef std::map<std::string, std::map<std::string, Chat*> > ProtocolChatPointerMap;
  typedef std::map<std::string, std::map<std::string, GridsID> > ProtocolChatIDMap;

  class OutsideChatController : public QObject {
    Q_OBJECT
  public:
    OutsideChatController(Device* d, QObject* parent = 0);
    void init();

    static GridsID requestAddAccount(Device* d, std::string protocol,
                                     std::string screen_name,
                                     std::string password);

    // This sends a message to someone on an outside chat protocol
    static GridsID requestOutsideChat(Device* dev,
                                      std::string protocol,
                                      std::string your_screen_name,
                                      std::string other_screen_name,
                                      std::string message);

    // This create a node from a message from an outside chat protocol
    static void gridsCreateOutsideChat(Device* d, Grids::Event* event);

    //std::map<std::string, std::string>& screen_names();
    //std::map<std::string, GridsID>& chat_ids();
    //ProtocolChatPointerMap& chat_pointers();
    ProtocolChatIDMap& chat_ids();

  private:
    Device* d_;
    // Maps protocol to screen name
    //std::map<std::string, std::string> screen_names_;
    //std::map<std::string, GridsID> chat_ids_;


    //ProtocolChatPointerMap chat_pointers_;
    ProtocolChatIDMap chat_ids_;


  };
}

#endif // OUTSIDECHATCONTROLLER_H
