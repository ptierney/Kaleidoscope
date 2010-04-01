#ifndef VIEW2D_H
#define VIEW2D_H

#include <QtGui/QGraphicsView>

namespace Kaleidoscope {
    class Scene2D;
    class Device;

    class View2D : public QGraphicsView {
        Q_OBJECT

    public:
        View2D(Device*, Scene2D*);

        void scaleView(qreal scaleFactor);

    protected:
        void wheelEvent(QWheelEvent *event);
        void keyPressEvent(QKeyEvent *event);

        void drawBackground(QPainter *painter, const QRectF &rect);

        /* This is set in the constructor. */
        QRect world_size;
    private:
        Device* d_;
    };
}

#endif // VIEW2D_H
