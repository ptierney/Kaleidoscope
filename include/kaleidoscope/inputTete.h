#ifndef INPUTTETE_H
#define INPUTTETE_H

// InputTetes are tomporary objects, they can't take
// links, nor can they become part of the final chat data structure

#include <string>

#include <grids/gObject.h>

namespace Kaleidoscope {
  class Device;

  class InputTete : Grids::Object {
  public:
    InputTete(Device*, Grids::Value*);
    void init();

    static GridsID requestCreate(Device* dev,
                                 GridsID parent,
                                 GridsID chat);
    static void gridsCreate(Device* dev, Grids::Event* evt);

    void appendText(std::string);
    void set_text(std::string);

  protected:
    GridsID getChatIDFromAttr(const Grids::Value&);
    GridsID getParentIDFromAttr(const Grids::Value&);

  private:
    Device* d_;
    std::string text_;
    GridsID chat_id_;
    GridsID parent_id_;
  };
}

#endif // INPUTTETE_H
