#ifndef VIEW2D_H
#define VIEW2D_H

#include <QtGui/QGraphicsView>

namespace Kaleidoscope {
    class Scene2D;

    class View2D : public QGraphicsView {
        Q_OBJECT

    public:
        View2D(Scene2D*);

    protected:
        void wheelEvent(QWheelEvent *event);
        void scaleView(qreal scaleFactor);
    };
}

#endif // VIEW2D_H
