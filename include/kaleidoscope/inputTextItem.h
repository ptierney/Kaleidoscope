#ifndef INPUTTEXTITEM_H
#define INPUTTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>

#include <kaleidoscope/define.h>
#include <kaleidoscope/object.h>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

namespace Kaleidoscope {

    class InputTextItem : public QGraphicsTextItem, public Object {
        Q_OBJECT

    public:
       enum { Type = UserType + 3 };

        InputTextItem(Device*, Grids::Value*, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

        static GridsID requestCreate( Device* dev, Vec3D position);
        static void gridsCreate(Device* dev, Grids::Event* evt);

        /* Required functions from Kal::Object */
        void draw(Device*);

        int type() const
            { return Type; }

    signals:
        void lostFocus(InputTextItem *item);
        void selectedChange(QGraphicsItem *item);

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void focusOutEvent(QFocusEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


    private:
        Device* d;
    };
}



#endif // INPUTTEXTITEM_H
