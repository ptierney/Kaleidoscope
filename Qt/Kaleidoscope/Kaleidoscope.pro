# -------------------------------------------------
# Project created by QtCreator 2009-09-04T01:51:13
# -------------------------------------------------
CONFIG += static
CXXFLAGS+=-fexceptions
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
    ../../src/kaleidoscope/consoleWindow.cpp \
    ../../src/kaleidoscope/camera.cpp \
    ../../src/grids/protocol.cpp \
    ../../src/grids/objectController.cpp \
    ../../src/grids/interface.cpp \
    ../../src/grids/event.cpp \
    ../../src/kaleidoscope/spaceRenderer.cpp \
    ../../src/grids/gObject.cpp \
    ../../src/json/json_writer.cpp \
    ../../src/json/json_value.cpp \
    ../../src/json/json_reader.cpp \
    ../../src/grids/utility.cpp \
    ../../src/kaleidoscope/spaceObject.cpp \
    ../../src/kaleidoscope/spaceText.cpp \
    ../../src/kaleidoscope/text.cpp \
    ../../src/kaleidoscope/inputTextItem.cpp \
    ../../src/kaleidoscope/scene2d.cpp \
    ../../src/kaleidoscope/genericNodeItem.cpp \
    ../../src/kaleidoscope/view2d.cpp
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
    ../../include/kaleidoscope/consoleWindow.h \
    ../../include/kaleidoscope/camera.h \
    ../../include/grids/vector3d.h \
    ../../include/grids/vector2d.h \
    ../../include/grids/utility.h \
    ../../include/grids/rect.h \
    ../../include/grids/protocol.h \
    ../../include/grids/position2d.h \
    ../../include/grids/plane3d.h \
    ../../include/grids/objectController.h \
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
    ../../include/grids/aabbox3d.h \
    ../../include/kaleidoscope/spaceRenderer.h \
    ../../include/grids/gObject.h \
    ../../include/json/writer.h \
    ../../include/json/value.h \
    ../../include/json/reader.h \
    ../../include/json/json.h \
    ../../include/json/forwards.h \
    ../../include/json/config.h \
    ../../include/json/autolink.h \
    ../../src/json/json_batchallocator.h \
    ../../include/kaleidoscope/spaceObject.h \
    ../../include/kaleidoscope/spaceText.h \
    ../../include/kaleidoscope/text.h \
    ui_kaleidoscope.h \
    ../../include/kaleidoscope/inputTextItem.h \
    ../../include/kaleidoscope/scene2d.h \
    ../../include/kaleidoscope/genericNodeItem.h \
    ../../include/kaleidoscope/view2d.h
FORMS += kaleidoscope.ui
