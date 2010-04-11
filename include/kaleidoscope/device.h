
#pragma once

#include <kaleidoscope/define.h>
#include <grids/define.h>

#include <QMainWindow>
#include <QApplication>
#include <QCursor>
#include <QTime>
#include <QTimer>
#include <QMutex>
#include <QObject>

#include <vector>

namespace Grids{
    class ObjectController;
    class Interface;
    class Utility;
    class Event;
}

namespace Kaleidoscope {
    class Room;
    class EventController;
    class Camera;
    class Settings;
    class NoticeWindow;
    class Console;
    class Scene2D;
    class View2D;
    class ChatController;
    class OutsideChatController;
    class User;

    class Device : public QObject {
        Q_OBJECT

    public:
        Device(QApplication*, QMainWindow*);
        ~Device();

        void init();
        Grids::ObjectController* getObjectController();
        Grids::Interface* getInterface();
        Grids::Utility* getGridsUtility();
        EventController* getEventController();
        EventController* event_controller();
        Settings* getSettings();
        Camera* getCamera();
        void registerCamera(Grids::Value*);
        ChatController* getChatController();
        ChatController* chat_controller();
        OutsideChatController* outside_chat_controller();
        User* user();

        void quit();
        bool getRunning();
        void setRunning( bool );
        GridsID getMyID();
        GridsID my_id();
        GridsID getMyRoom();
        void setMyID(GridsID);
        void setMyRoom(GridsID);
        int getTicks();
        NoticeWindow* getNoticeWindow();
        NoticeWindow* getErrorWindow();
        Scene2D* getScene();
        void registerNotice(QObject*);
        void registerError(QObject*);
        void createSpaceRenderer();
        void createUserInputWindow();
        void createAddUserWindow();


        QMainWindow* main_window;
        // Cameras vs Renderers:
        // There is only 1 renderer of the program, because there is only one
        // "space"/"world" of the program.  However, there can be many views
        // into this space, and each view is represented by a camera.
        // Each view requires a camera, which sets up the viewport.  The renderer
        // (the same renderer for each camera), then draws the world.
        // Cameras are grids objects, which have IDs and icons in the world,
        // and must be requested through the Interface.
        // The renderer is the current OpenGL means of displaying the world
        // on the computer screen.
        Camera* main_camera;
        QApplication* app;

        /* Returns true of the item is a [sweet] child of mine. */
        bool myChild(GridsID);

    protected:
        void timerEvent(QTimerEvent *event);

    public slots:
        void gridsConnectionEstablished();
        void myRoomCreated(GridsID);

    private:
        Grids::ObjectController* object_controller;
        Grids::Interface* g_interface;
        Grids::Utility* g_utility;

        EventController* event_controller_;
        Settings* settings;
        Console* console;
        NoticeWindow* noticeWindow;
        NoticeWindow* errorWindow;
        Scene2D* scene;
        View2D* view2D;
        ChatController* chat_controller_;
        OutsideChatController* outside_chat_controller_;
        User* user_;

        void createObjects();

        void loadRoom();
        void requestCreateCamera();
        // The three main starting boxes
        void createMainCamera();
        void createConsole();
        void createNoticeWindow();
        void createErrorWindow();
        /* The scene is the area you draw on. */
        void createScene();

        QTime qtime;
        QTimer *timer;
        GridsID my_id_;
        GridsID my_room;
        bool connected;

        bool running;
        QMutex running_mutex;
        QMutex my_id_mutex;
        QMutex my_room_mutex;

        bool debug_display_;
    };

} // end namespace Kaleidoscope
