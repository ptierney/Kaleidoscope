
#include <kaleidoscope/console.h>
#include <kaleidoscope/define.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/genericNodeItem.h>
#include <kaleidoscope/genericLinkItem.h>
#include <grids/interface.h>
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
                GenericLinkItem::requestCreate(d, node1, node2, d->getMyID());
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
        }

    }
} // end namespace Kaleidoscope
