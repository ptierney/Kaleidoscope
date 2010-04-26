# -------------------------------------------------
# Project created by QtCreator 2009-09-04T01:51:13
# -------------------------------------------------
CONFIG += static
CXXFLAGS += -fexceptions
QT += network \
    opengl
TARGET = Kaleidoscope
TEMPLATE = app
ICON = kaleidoscope.icns
RC_FILE = kaleidoscope.rc
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
    ../../src/kaleidoscope/view2d.cpp \
    ../../src/kaleidoscope/genericLinkItem.cpp \
    ../../src/kaleidoscope/tete.cpp \
    ../../src/kaleidoscope/user.cpp \
    ../../src/kaleidoscope/teteNode.cpp \
    ../../src/kaleidoscope/respondNode.cpp \
    ../../src/kaleidoscope/textNode.cpp \
    ../../src/kaleidoscope/displayTextNode.cpp \
    ../../src/kaleidoscope/inputTextNode.cpp \
    ../../src/kaleidoscope/chatController.cpp \
    ../../src/kaleidoscope/chat.cpp \
    ../../src/kaleidoscope/chatLinkSystem.cpp \
    ../../src/kaleidoscope/chatNode.cpp \
    ../../src/kaleidoscope/frameRect.cpp \
    ../../src/kaleidoscope/link.cpp \
    ../../src/kaleidoscope/linkNode.cpp \
    ../../src/kaleidoscope/inputTete.cpp \
    ../../src/kaleidoscope/displayTextItem.cpp \
    ../../src/kaleidoscope/userNameInputBox.cpp \
    ../../src/kaleidoscope/userNameBox.cpp \
    ../../src/kaleidoscope/activityCircle.cpp \
    ../../src/kaleidoscope/addUsernameBox.cpp \
    ../../src/kaleidoscope/outsideChatController.cpp \
    ../../src/kaleidoscope/nodeGradient.cpp \
    ../../src/kaleidoscope/linkCreator.cpp \
    ../../src/kaleidoscope/nameTextItem.cpp \
    ../../src/kaleidoscope/otherUsersNode.cpp \
    ../../src/kaleidoscope/usersScene.cpp \
    ../../src/kaleidoscope/usersView.cpp \
    ../../src/kaleidoscope/userIcon.cpp \
    ../../src/kaleidoscope/usersWidget.cpp
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
    ../../include/kaleidoscope/view2d.h \
    ../../include/kaleidoscope/genericLinkItem.h \
    ../../include/kaleidoscope/tete.h \
    ../../include/kaleidoscope/user.h \
    ../../include/kaleidoscope/teteNode.h \
    ../../include/kaleidoscope/respondNode.h \
    ../../include/kaleidoscope/textNode.h \
    ../../include/kaleidoscope/displayTextNode.h \
    ../../include/kaleidoscope/inputTextNode.h \
    ../../include/kaleidoscope/chatController.h \
    ../../include/kaleidoscope/chat.h \
    ../../include/kaleidoscope/chatLinkSystem.h \
    ../../include/kaleidoscope/chatNode.h \
    ../../include/kaleidoscope/frameRect.h \
    ../../include/kaleidoscope/link.h \
    ../../include/kaleidoscope/linkNode.h \
    ../../include/kaleidoscope/inputTete.h \
    ../../include/kaleidoscope/teteInterface.h \
    ../../include/kaleidoscope/displayTextItem.h \
    ../../include/kaleidoscope/userNameInputBox.h \
    ../../include/kaleidoscope/userNameBox.h \
    ../../include/kaleidoscope/activityInterface.h \
    ../../include/kaleidoscope/activityCircle.h \
    ../../include/kaleidoscope/registerUsernameBox.h \
    ../../include/kaleidoscope/addUsernameBox.h \
    ../../include/kaleidoscope/outsideChatController.h \
    ../../include/kaleidoscope/outsideChat.h \
    ../../include/kaleidoscope/nodeGradient.h \
    ../../include/kaleidoscope/linkCreator.h \
    ../../include/kaleidoscope/nameTextItem.h \
    ../../include/kaleidoscope/otherUsersNode.h \
    ../../include/kaleidoscope/usersScene.h \
    ../../include/kaleidoscope/usersView.h \
    ../../include/kaleidoscope/userIcon.h \
    ../../include/kaleidoscope/usersWidget.h
FORMS += kaleidoscope.ui
