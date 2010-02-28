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

        void drawBackground(QPainter *painter, const QRectF &rect);

        /* This is set in the constructor. */
        QRect world_size;
    };
}

#endif // VIEW2D_H
