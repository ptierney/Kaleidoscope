
#include <kaleidoscope/console.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/genericLinkItem.h>
#include <kaleidoscope/inputTextNode.h>
#include <kaleidoscope/scene2d.h>
#include <grids/interface.h>
#include <kaleidoscope/chat.h>
#include <kaleidoscope/tete.h>
#include <kaleidoscope/chatController.h>
#include <kaleidoscope/device.h>


namespace Kaleidoscope {

    Console::Console(Device* d, QWidget* window_parent){
        this->d = d;
	
        console_window = new ConsoleWindow(window_parent);

        // link returnPressed with newInput

        QObject::connect(console_window, SIGNAL( returnPressed() ),
                         this, SLOT(newInput() ) );

        QObject::connect(this, SIGNAL( clearLine() ),
                         console_window, SLOT( clear() ));
    }

    ConsoleWindow* Console::getConsoleWindow() {
        return console_window;
    }

    void Console::newInput() {
        QString input_text = console_window->text();

        d->getNoticeWindow()->write(tr("> ") + input_text);
        clearLine();

        parseInput(input_text);
    }

    void Console::parseInput(QString input){
        QString lower = input.toLower();
        QStringList input_parse = lower.split(tr(" "));
        QString first = input_parse[0];
        QString second;
        if(input_parse.length() > 1)
            second = input_parse[1];

        if(lower == tr("connect") ||
           lower == tr("conect to server") ){
            connectToServer();
        } else if(lower == tr("exit") ||
                  lower == tr("quit") ){
            d->quit();
        } else if(lower == tr("create room") ){
            d->getInterface()->createMyRoom();
            //createRoom();
        } else if(lower == tr("join room")) {
            d->getInterface()->requestAllRooms();
        } else if(first == tr("create")) {
            if(second == tr("node")) {
                GenericNodeItem::requestCreate(d, Vec3D(1.0, 1.0, 1.0), "Generic Node");
            } else if(second == tr("nodes")) {
                GridsID node1 = GenericNodeItem::requestCreate(d, Vec3D(300.0, 150.0, 0.0), "Node 1");
                GridsID node2 = GenericNodeItem::requestCreate(d, Vec3D(-300.0, -150.0, 0.0), "Node 2");
                GenericLinkItem::requestCreate(d, node1, node2, GenericLinkItem::SOFT_LINK, d->getMyID());
                GridsID node3 = GenericNodeItem::requestCreate(d, Vec3D(300.0, -150.0, 0.0), "Node 3");
                GenericLinkItem::requestCreate(d, node1, node3, GenericLinkItem::HARD_LINK, d->getMyID());

              } else if(second == tr("inputnode")){
                InputTextNode* input_node = new InputTextNode(d);
                input_node->init();
                d->getScene()->addItem(input_node);
              }
        } else if(first == tr("notify")) {
            d->getNoticeWindow()->setPriority(second.toInt());
        } else if(first == tr("get")) {
            if(second == tr("myroom")){
                d->getNoticeWindow()->write(10, tr("Your room = ") + tr(d->getMyRoom().c_str()));
            } else if(second == tr("myid")) {
                d->getNoticeWindow()->write(10, tr("Your id = ") + tr(d->getMyID().c_str()));
            }
        } else if(first == tr("request")) {
            if(second == tr("allrooms")) {
                d->getInterface()->requestAllRooms();
            }
        } else if(first == tr("length")) {
            if(second == tr("outboundqueue")) {
                QString temp_string;
                temp_string.setNum(d->getInterface()->getOutboundQueueLength());
                d->getNoticeWindow()->write(10, tr("Outbound queue has: ") + temp_string);
            } else if(second == tr("inboundqueue")) {
                QString temp_string;
                temp_string.setNum(d->getInterface()->getReceivedQueueLength());
                d->getNoticeWindow()->write(10, tr("Received queue has: ") + temp_string);
            }
          } else if(first == tr("update")){
            if(second == tr("node")){
              //d->chat_controller()->chats()[0u]->tetes()[0u]->tete_node()->setPos(0.0, 0.0);
              d->chat_controller()->chats()[0u]->tetes()[0u]->tete_node()->updatePosition();

            }
          }

    }
} // end namespace Kaleidoscope
