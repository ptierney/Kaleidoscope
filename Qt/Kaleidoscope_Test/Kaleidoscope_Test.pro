# -------------------------------------------------
# Project created by QtCreator 2009-09-02T16:45:01
# -------------------------------------------------
QT += network \
    opengl \
    script \
    svg \
    xml \
    testlib
TARGET = Kaleidoscope_Test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    protocoltest.cpp \
    ../../src/json/json_writer.cpp \
    ../../src/json/json_value.cpp \
    ../../src/json/json_reader.cpp \
    ../../src/grids/protocol.cpp \
    ../../src/grids/event.cpp
INCLUDEPATH += ../../include
HEADERS += ../../src/json/json_batchallocator.h \
    ../../include/grids/protocol.h \
    ../../include/grids/define.h \
    ../../include/grids/event.h
