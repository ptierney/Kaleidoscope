#ifndef ACTIVITYCIRCLE_H
#define ACTIVITYCIRCLE_H

#include <QGraphicsItem>

namespace Kaleidoscope {
  class Tete;

  class ActivityCircle : public QGraphicsItem, public ActivityInterface {

  public:
    ActivityCircle(QGraphicsItem* parent = 0, Tete* tete);

    Tete* tete();

  private:
    Tete* tete_;



  };


}

#endif // ACTIVITYCIRCLE_H
