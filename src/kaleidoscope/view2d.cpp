
#include <kaleidoscope/view2d.h>
#include <kaleidoscope/scene2d.h>

#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

namespace Kaleidoscope {

    View2D::View2D(Scene2D *scene) : QGraphicsView(scene) {

        scene->setItemIndexMethod(QGraphicsScene::NoIndex);

        //world_size = QRect(-10000, -10000, 10000, 10000).normalized();
        //scene->setSceneRect(world_size);
        //scene->setSceneRect(-200, -200, 200, 200);

        setScene(scene);

        setCacheMode(CacheBackground);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setRenderHint(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setResizeAnchor(AnchorViewCenter);

        setDragMode( QGraphicsView::ScrollHandDrag);

        scale(qreal(0.8), qreal(0.8));
        setMinimumSize(400, 400);
        //centerOn(10000.0, 10000.0);
    }

    void View2D::drawBackground(QPainter *painter, const QRectF &rect)
    {
        Q_UNUSED(rect);
        /* This is ugly
        // Shadow
        QRectF sceneRect = this->sceneRect();
        QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
        QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
        if (rightShadow.intersects(rect) || rightShadow.contains(rect))
            painter->fillRect(rightShadow, Qt::darkGray);
        if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
            painter->fillRect(bottomShadow, Qt::darkGray);

        // Fill
        QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
        gradient.setColorAt(0, Qt::white);
        gradient.setColorAt(1, Qt::lightGray);
        painter->fillRect(rect.intersect(sceneRect), gradient);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(sceneRect);
        */
    }

    void View2D::wheelEvent(QWheelEvent *event) {
        scaleView(pow((double)2, event->delta() / 240.0));
    }

    void View2D::scaleView(qreal scale_factor) {
        qreal factor = matrix().scale(scale_factor, scale_factor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scale_factor, scale_factor);
    }

}
