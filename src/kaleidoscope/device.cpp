
#include <time.h>

#include <iostream>

#include <QDockWidget>
#include <QGraphicsView>

#include <kaleidoscope/device.h>
#include <kaleidoscope/settings.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/console.h>
#include <kaleidoscope/scene2d.h>
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/chatController.h>

#include <grids/interface.h>
#include <grids/objectController.h>
#include <grids/utility.h>
#include <grids/event.h>

namespace Kaleidoscope {
    Device::Device(QApplication* in_app, QMainWindow* m_win)
        : QObject(m_win) {
        main_window = m_win;
        app = in_app;
        main_camera = 0;
        connected = 0;
        qtime.start();
        qsrand ( time(NULL) );

        /* OK, let's try setting up an event loop like this. */
        startTimer(100);

        /* NoticeWindow & ErrorWindow must be created before any Grids::Objects,
         * as all Grids::Objects require them to be created. */
        createNoticeWindow();
        createErrorWindow();
        createConsole();
        createScene();
        /* Show the main window so we can read notices as the program connects. */
        m_win->show();
	
        noticeWindow->addNotice(tr("Set up windows"));

        createObjects();

        setMyID( getGridsUtility()->getNewUUID() );

        init();
    }

    void Device::createObjects(){
        noticeWindow->addNotice(tr("Creating objects"));

        // Settings stores / loads the user data
        settings = new Settings();
        chat_controller_ = new ChatController(this);
        chat_controller_->init();

        // Derived from QWidget, handles misc. key presses
        event_controller = new Kal::EventController(this, main_window);
        object_controller = new Grids::ObjectController(this, main_window);
        g_utility = new Grids::Utility();
    }

    void Device::init() {
        getNoticeWindow()->write(0, tr("Initializing..."));
        // Creates the protocol, connects to the server
        g_interface = new Grids::Interface(this, main_window);
        getNoticeWindow()->write(0, tr("Created Interface"));
    }
    void Device::quit() {
        app->exit(0);
    }

    /* This is the main event loop. */
    void Device::timerEvent(QTimerEvent *event) {

        if(connected == false && getInterface()->isConnected() == true) {
            connected = true;
            getNoticeWindow()->write(tr("Connected!"));
            getNoticeWindow()->write(0, tr("Loading room / creating room"));
            loadRoom();
        }

        getInterface()->flushProtocol();
        getInterface()->collectEvents();
    }

    void Device::gridsConnectionEstablished() {
        loadRoom();
    }

    Device::~Device() {
        /* Any Qt derived object is destroyed by Qt */
        /*
         */
        /* If I delete the interface here, it closes down the
           protocol thread. */
        delete g_interface;
        delete g_utility;
        delete settings;
    }

    Grids::ObjectController* Device::getObjectController(){ return object_controller; }
    Grids::Interface* Device::getInterface(){ return g_interface; }
    Grids::Utility* Device::getGridsUtility(){ return g_utility; }
    Settings* Device::getSettings() { return settings; }
    EventController* Device::getEventController(){ return event_controller; }
    NoticeWindow* Device::getNoticeWindow() { return noticeWindow; }
    NoticeWindow* Device::getErrorWindow() { return errorWindow; }
    Scene2D* Device::getScene() { return scene; }
    Camera* Device::getCamera() { return main_camera; }
    ChatController* Device::getChatController() { return chat_controller_; }
    ChatController* Device::chat_controller() { return chat_controller_; }

    void Device::loadRoom(){
        getInterface()->requestAllRooms();
    }

    void Device::myRoomCreated(GridsID rm) {
        //getNoticeWindow()->addNotice(4, tr("Your room created"));
        //getNoticeWindow()->addNotice(4, tr("Requesting a camera from the server."));

        /* Requests a new camera from the server. */
        //requestCreateCamera();
    }

    // The four main starting boxes: camera, notices, errors, console

    // Create a new camera, and if it belongs to us, register it as the main
    // camera.
    void Device::registerCamera(Grids::Value* val) {
        QDockWidget *dock = new QDockWidget(tr("Camera"), main_window);
        Camera* cam = new Camera(this, val, dock);
        dock->setWidget(cam);
        main_window->addDockWidget(Qt::LeftDockWidgetArea, dock);
        //main_window->setCentralWidget(cam);

        if(main_camera == 0)
            main_camera = cam;
    }

    void Device::createConsole() {
        QDockWidget *dock = new QDockWidget(tr("Console"), main_window);
        dock->setFloating(0);
        dock->resize(DEFAULT_SIDEBAR_WIDTH, DEFAULT_WINDOW_HEIGHT/2.f);
        console = new Console(this, dock);
        dock->setWidget(console->getConsoleWindow());

        main_window->addDockWidget( Qt::RightDockWidgetArea, dock);
    }

    void Device::createNoticeWindow() {
        QDockWidget *dock = new QDockWidget(tr("Notices"), main_window);
        dock->setFloating(0);
        dock->resize(DEFAULT_SIDEBAR_WIDTH, DEFAULT_WINDOW_HEIGHT/2.f);
        noticeWindow = new NoticeWindow(dock);
        dock->setWidget(noticeWindow);
        main_window->addDockWidget(Qt::RightDockWidgetArea, dock);
    }

    void Device::createErrorWindow() {
        QDockWidget *dock = new QDockWidget(tr("Errors"), main_window);
        dock->setFloating(0);
        errorWindow = new NoticeWindow(dock);
        dock->setWidget(errorWindow);
        main_window->addDockWidget(Qt::RightDockWidgetArea, dock);
    }

    void Device::createScene() {

        scene = new Scene2D(this, main_window);
        view2D = new View2D(scene);

        main_window->setCentralWidget(view2D);
    }

    void Device::registerNotice(QObject* object) {
        noticeWindow->connect(object, SIGNAL(notice(int, QString)),
                              noticeWindow, SLOT(addNotice(int,QString)));
    }

    void Device::registerError(QObject* object) {
        errorWindow->connect(object, SIGNAL(error(int, QString)),
                             errorWindow, SLOT(addNotice(int,QString)));
    }

    void Device::setMyRoom(GridsID new_room) {
        my_room = new_room;
        getInterface()->setMyRoom(new_room);
    }

    bool Device::myChild(GridsID test_id) {
        return test_id == getMyID();
    }

    int Device::getTicks() {
        return qtime.elapsed();
    }

    void Device::requestCreateCamera(){
        Vec3D start_pos = Vec3D( 10.0f, 30.0f, 35.0f );
        Vec3D start_rot = Vec3D( 1.0f, -1.0f, 0.0f );
        Vec3D start_scl = Vec3D( 1.0f, 1.0f, 1.0f );

        Grids::Value* cam_val = new Grids::Value();

        (*cam_val)[ "type" ] = "Camera";
        (*cam_val)[ "parent" ] = getMyID();
        (*cam_val)[ "camera_type" ] = MAYA;
        (*cam_val)[ "rotate_speed" ] = .3f;
        (*cam_val)[ "translate_speed" ] = 0.001f;
        (*cam_val)[ "target" ][ 0u ] = -1.0f;
        (*cam_val)[ "target" ][ 1u ] = -1.0f;
        (*cam_val)[ "target" ][ 2u ] = -1.0f;
        (*cam_val)[ "up" ][ 0u ] = 0.0f;
        (*cam_val)[ "up" ][ 1u ] = 1.0f;
        (*cam_val)[ "up" ][ 2u ] = 0.0f;

        getInterface()->requestCreateObject( cam_val, start_pos, start_rot, start_scl );

        delete cam_val;
    }

    /////////////////////////////////////
    // Accessor Functions

    bool Device::getRunning() {
        //QMutexLocker lock(&running_mutex);
        return running;
    }

    void Device::setRunning( bool new_run ){
        //QMutexLocker lock(&running_mutex);
        running = new_run;
    }

    GridsID Device::getMyID(){
        //QMutexLocker lock(&my_id_mutex);
        return my_id;
    }

    void Device::setMyID( GridsID temp_id ){
        //QMutexLocker lock(&my_id_mutex);
        my_id = temp_id;
        getNoticeWindow()->addNotice(4, tr("ID set to: ")+tr(temp_id.c_str()));
    }

    GridsID Device::getMyRoom() {
        return getInterface()->getMyRoom();
    }
} // end namespace Kaleidoscope
