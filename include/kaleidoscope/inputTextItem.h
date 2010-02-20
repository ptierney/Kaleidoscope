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

        void setLocalPosition(Vec3D);

        int type() const
            { return Type; }

        void updateText(QString new_text);

        /* Overlead the updateAttr function from Grids::Object.
           If this is not overloaded, the original updateAttr will clear
           the old attr. */
        void updateAttr(Grids::Event*);

    signals:
        void lostFocus(InputTextItem *item);
        void selectedChange(QGraphicsItem *item);

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void focusInEvent(QFocusEvent *event);
        void focusOutEvent(QFocusEvent *event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);    

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void keyPressEvent(QKeyEvent* event);

    private:
        Device* d;

        QPointF position_change;
        QMutex input_text_item_mutex;
        QMutex focus_mutex;
    };
}



#endif // INPUTTEXTITEM_H
