#ifndef SCENE2D_H
#define SCENE2D_H

#include <QGraphicsScene>
#include <grids/define.h>
#include <kaleidoscope/define.h>


QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE


namespace Kaleidoscope {
    class InputTextItem;
    class Device;

    class Scene2D : public QGraphicsScene {
        Q_OBJECT

    public:

        Scene2D(Device*, QObject* parent = 0);

        void addInputTextItem(Grids::Value*, Vec3D);

        QFont font() const
        { return myFont; }
        QColor textColor() const
        { return myTextColor; }

        void setTextColor(const QColor &color);
        void setFont(const QFont &font);  

    public slots:
        void editorLostFocus(InputTextItem *item);

    signals:
        void textInserted(QGraphicsTextItem *item);
        void itemSelected(QGraphicsItem *item);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


    private:
        Device* d;
        InputTextItem* textItem;
        QColor myTextColor;
        QFont myFont;

    };

}


#endif // SCENE2D_H
