
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>

#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

namespace Kaleidoscope {

    View2D::View2D(Scene2D *scene) : QGraphicsView(scene) {

        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        scene->setSceneRect(-200, -200, 400, 400);
        setScene(scene);

        setCacheMode(CacheBackground);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setResizeAnchor(AnchorViewCenter);

        scale(qreal(0.8), qreal(0.8));
        setMinimumSize(400, 400);
    }

    void View2D::wheelEvent(QWheelEvent *event) {
        scaleView(pow((double)2, -event->delta() / 240.0));
    }

    void View2D::scaleView(qreal scale_factor) {
        qreal factor = matrix().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scale_factor, scale_factor);
    }

}
