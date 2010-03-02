#ifndef INPUTTEXTITEM_H
#define INPUTTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>
#include <QTime>

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

        void updateText(QString new_text);

        /* Overlead the updateAttr function from Grids::Object.
           If this is not overloaded, the original updateAttr will clear
           the old attr. */
        void updateAttr(Grids::Event*);

        /* Overload Grids::Object function */
        void setLocalPosition(Vec3D);

        QColor getTextColor();

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

        void timerEvent(QTimerEvent *);

    private:
        /* Adds a newline to the text,
           and updates the timers so the
           text is sent. */
        void appendNewline();

        Device* d;

        QPointF position_change;
        QMutex input_text_item_mutex;
        QMutex focus_mutex;

        //const QColor text_color = QColor(6,68,193,200);
        static const int text_color_r = 6;
        static const int text_color_g = 68;
        static const int text_color_b = 193;
        static const int text_color_a = 220;

        QColor text_color;

        QTime key_timer;
        QTime last_key_press;
        /* Keys unsent is for general updates
           to the servers.  Input is for newline
           terminated messages that register as
           input. */
        bool keys_unsent;
        bool input_unsent;
        int key_delay;
        int input_time;
    };
}



#endif // INPUTTEXTITEM_H
