
#include <kaleidoscope/device.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/settings.h>
#include <kaleidoscope/spaceRenderer.h>
#include <kaleidoscope/noticeWindow.h>
#include <kaleidoscope/consoleWindow.h>
#include <kaleidoscope/console.h>
#include <kaleidoscope/scene2d.h>
#include <grids/objectController.h>
#include <grids/interface.h>
#include <grids/utility.h>
#include <grids/event.h>
#include <iostream>
#include <QDockWidget>
#include <QGraphicsView>

namespace Kaleidoscope {
    Device::Device(QApplication* in_app, QMainWindow* m_win)
        : QObject(m_win) {
        main_window = m_win;
        app = in_app;
        main_camera = 0;
        time.start();

        // NoticeWindow & ErrorWindow must be created before any Grids::Objects,
        // as all Grids::Objects require them to be created
        createNoticeWindow();
        createErrorWindow();
        createConsole();
        createScene();
        // Show the main window so we can read notices as the program connects.
        m_win->show();
	
        noticeWindow->addNotice(tr("Set up windows"));

        createObjects();

        setMyID( "7A293FB2-70C9-11DE-B84C-43FC4C661FD7" );

        init();
    }

    void Device::createObjects(){
        noticeWindow->addNotice(tr("Creating objects"));

        // Settings stores / loads the user data
        settings = new Settings();

        // Derived from QWidget, handles misc. key presses
        event_controller = new Kal::EventController(this, main_window);
        object_controller = new Grids::ObjectController(this, main_window);
        g_utility = new Grids::Utility();

        createSpaceRenderer();
    }

    void Device::init() {
        getNoticeWindow()->write(0, tr("Initializing..."));
        // Creates the protocol, connects to the server
        interface = new Grids::Interface(this, main_window);
        getNoticeWindow()->write(0, tr("Created Interface"));
    }

    void Device::quit() {
        app->exit(0);
    }

    void Device::gridsConnectionEstablished() {
        getNoticeWindow()->write(0, tr("Loading room / creating room"));
        loadRoom();
    }

    Device::~Device() {
        // Most of the created devices are deleted by Qt
        delete g_utility;
        // Window should be one of the last objects to be destroyed, as it calls SDL_Quit()
        delete settings;
    }

    Grids::ObjectController* Device::getObjectController(){ return object_controller; }
    Grids::Interface* Device::getInterface(){ return interface; }
    Grids::Utility* Device::getGridsUtility(){ return g_utility; }
    Settings* Device::getSettings() { return settings; }
    EventController* Device::getEventController(){ return event_controller; }
    NoticeWindow* Device::getNoticeWindow() { return noticeWindow; }
    NoticeWindow* Device::getErrorWindow() { return errorWindow; }
    Scene2D* Device::getScene() { return scene; }
    SpaceRenderer* Device::getRenderer() { return renderer; }
    Camera* Device::getCamera() { return main_camera; }

    void Device::loadRoom(){
        getInterface()->requestAllRooms();
    }

    void Device::myRoomCreated(GridsID rm) {
        getNoticeWindow()->addNotice(4, tr("Your room created"));

        std::cerr << "Requesting create camera\n";
        // Requests a new camera from the server
        // Creates and registers a new rendere
        requestCreateCamera();
    }

    // The four main starting boxes: camera, notices, errors, console
    //

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
        main_window->addDockWidget(Qt::BottomDockWidgetArea, dock);
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
        /*
        QDockWidget *dock = new QDockWidget(tr("Scene"), main_window);
        dock->setFloating(0);
        */
        scene = new Scene2D(this, main_window);
        QGraphicsView* view = new QGraphicsView(scene);
        /*
        dock->setWidget(view);
        main_window->addDockWidget(Qt::LeftDockWidgetArea, dock);
        */
        main_window->setCentralWidget(view);
    }

    void Device::registerNotice(QObject* object) {
        noticeWindow->connect(object, SIGNAL(notice(int, QString)),
                              noticeWindow, SLOT(addNotice(int,QString)));
    }

    void Device::registerError(QObject* object) {
        errorWindow->connect(object, SIGNAL(error(int, QString)),
                             errorWindow, SLOT(addNotice(int,QString)));
    }

    void Device::receiveRoomList(Grids::Event* evt) {
        Grids::Value* args = evt->getArgsPtr();

        if( (*args)["rooms"][0u].empty() )
            return;

        setMyRoom((*args)["rooms"][0u].asString());
        getNoticeWindow()->write(tr("Loaded a room"));
        getNoticeWindow()->write(tr((*args)["rooms"][0u].asString().c_str()));
    }

    void Device::setMyRoom(GridsID new_room) {
        my_room = new_room;
        getInterface()->setMyRoom(new_room);
    }


    int Device::getTicks() {
        return time.elapsed();
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

        interface->requestCreateObject( cam_val, start_pos, start_rot, start_scl );

        delete cam_val;
    }

    void Device::createSpaceRenderer(){
        Grids::Value* ren = new Grids::Value();

        (*ren)[ "id" ] = getGridsUtility()->getNewUUID();
        (*ren)[ "req" ][ "pos" ][ 0u ] = 0.0f;
        (*ren)[ "req" ][ "pos" ][ 1u ] = 0.0f;
        (*ren)[ "req" ][ "pos" ][ 2u ] = 0.0f;
        (*ren)[ "req" ][ "rot" ][ 0u ] = 0.0f;
        (*ren)[ "req" ][ "rot" ][ 1u ] = 0.0f;
        (*ren)[ "req" ][ "rot" ][ 2u ] = 0.0f;
        (*ren)[ "req" ][ "scl" ][ 0u ] = 1.0f;
        (*ren)[ "req" ][ "scl" ][ 1u ] = 1.0f;
        (*ren)[ "req" ][ "scl" ][ 2u ] = 1.0f;

        renderer = new SpaceRenderer(this, ren);
    }

    /////////////////////////////////////
    // Accessor Functions

    bool Device::getRunning() {
        QMutexLocker lock(&running_mutex);
        return running;
    }

    void Device::setRunning( bool new_run ){
        QMutexLocker lock(&running_mutex);
        running = new_run;
    }

    GridsID Device::getMyID(){
        QMutexLocker lock(&my_id_mutex);
        return my_id;
    }

    void Device::setMyID( GridsID temp_id ){
        QMutexLocker lock(&my_id_mutex);
        my_id = temp_id;
        getNoticeWindow()->addNotice(4, tr("ID set to: ")+tr(temp_id.c_str()));
    }

    GridsID Device::getMyRoom() {
        QMutexLocker lock(&my_room_mutex);
        return my_room;
    }
} // end namespace Kaleidoscope
