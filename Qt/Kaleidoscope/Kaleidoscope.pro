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
    ../../src/kaleidoscope/camera.cpp
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
    ../../include/kaleidoscope/camera.h
FORMS += kaleidoscope.ui
