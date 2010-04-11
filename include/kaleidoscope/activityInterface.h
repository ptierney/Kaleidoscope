#ifndef ACTIVITYINTERFACE_H
#define ACTIVITYINTERFACE_H

namespace Kaleidoscope {
  class Tete;

  class ActivityInterface {
  public:
    virtual ~ActivityInterface();

    Tete* tete() = 0;

  };
}

#endif // ACTIVITYINTERFACE_H
