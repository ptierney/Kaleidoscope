# -------------------------------------------------
# Project created by QtCreator 2009-09-02T00:16:51
# -------------------------------------------------
QT += network \
    opengl \
    svg \
    xml \
    xmlpatterns \
    testlib
TARGET = Kaleidoscope
TEMPLATE = app
SOURCES += main.cpp \
    kaleidoscope.cpp \
    ../../src/json/json_writer.cpp \
    ../../src/json/json_valueiterator.inl \
    ../../src/json/json_value.cpp \
    ../../src/json/json_reader.cpp \
    ../../src/json/json_internalmap.inl \
    ../../src/json/json_internalarray.inl \
    ../../src/grids/protocol.cpp
HEADERS += kaleidoscope.h \
    ../../include/json/writer.h \
    ../../include/json/value.h \
    ../../include/json/reader.h \
    ../../include/json/json.h \
    ../../include/json/forwards.h \
    ../../include/json/config.h \
    ../../include/json/autolink.h \
    ../../src/json/json_batchallocator.h \
    ../../include/grids/protocol.h \
    ../../include/grids/event.h \
    ../../include/grids/define.h
FORMS += kaleidoscope.ui
INCLUDEPATH += ../../include
