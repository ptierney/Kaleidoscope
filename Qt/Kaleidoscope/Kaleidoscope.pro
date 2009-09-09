# -------------------------------------------------
# Project created by QtCreator 2009-09-04T01:51:13
# -------------------------------------------------
QT += network \
    opengl \
    svg \
    webkit \
    xml \
    xmlpatterns \
    phonon
TARGET = Kaleidoscope
TEMPLATE = app
SOURCES += main.cpp \
    kaleidoscope.cpp \
    ../../src/kaleidoscope/settings.cpp \
    ../../src/kaleidoscope/room.cpp \
    ../../src/kaleidoscope/object.cpp \
    ../../src/kaleidoscope/noticeWindow.cpp \
    ../../src/kaleidoscope/glSpaceObject.cpp \
    ../../src/kaleidoscope/eventController.cpp \
    ../../src/kaleidoscope/device.cpp \
    ../../src/kaleidoscope/cursorController.cpp \
    ../../src/kaleidoscope/console.cpp \
    ../../src/kaleidoscope/camera.cpp \
    ../../src/grids/protocol.cpp \
    ../../src/grids/objectController.cpp \
    ../../src/grids/object.cpp \
    ../../src/grids/interface.cpp \
    ../../src/grids/event.cpp \
    ../../src/grids/protocol.cpp \
    ../../src/grids/objectController.cpp \
    ../../src/grids/object.cpp \
    ../../src/grids/interface.cpp \
    ../../src/grids/event.cpp
INCLUDEPATH += ../../include
HEADERS += kaleidoscope.h \
    ../../include/kaleidoscope/settings.h \
    ../../include/kaleidoscope/room.h \
    ../../include/kaleidoscope/object.h \
    ../../include/kaleidoscope/noticeWindow.h \
    ../../include/kaleidoscope/glSpaceObject.h \
    ../../include/kaleidoscope/eventController.h \
    ../../include/kaleidoscope/device.h \
    ../../include/kaleidoscope/define.h \
    ../../include/kaleidoscope/cursorController.h \
    ../../include/kaleidoscope/console.h \
    ../../include/kaleidoscope/camera.h \
    ../../include/grids/vector3d.h \
    ../../include/grids/vector2d.h \
    ../../include/grids/utility.h \
    ../../include/grids/rect.h \
    ../../include/grids/protocol.h \
    ../../include/grids/position2d.h \
    ../../include/grids/plane3d.h \
    ../../include/grids/objectController.h \
    ../../include/grids/object.h \
    ../../include/grids/matrix4.h \
    ../../include/grids/line3d.h \
    ../../include/grids/irrTypes.h \
    ../../include/grids/irrString.h \
    ../../include/grids/irrMath.h \
    ../../include/grids/IrrCompileConfig.h \
    ../../include/grids/irrAllocator.h \
    ../../include/grids/interface.h \
    ../../include/grids/event.h \
    ../../include/grids/dimension2d.h \
    ../../include/grids/define.h \
    ../../include/grids/aabbox3d.h
FORMS += kaleidoscope.ui
