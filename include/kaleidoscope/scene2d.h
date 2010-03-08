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
class QWheelEvent;
QT_END_NAMESPACE


namespace Kaleidoscope {
    class InputTextItem;
    class Device;
    class GenericNodeItem;
    class GenericLinkItem;
    class InputTextItem;

    class Scene2D : public QGraphicsScene {
        Q_OBJECT

    public:

        Scene2D(Device*, QObject* parent = 0);
        QSize sizeHint() const;


        QFont font() const
        { return myFont; }
        QColor textColor() const
        { return myTextColor; }

        void setTextColor(const QColor &color);
        void setFont(const QFont &font);

        void addInputTextItem(InputTextItem*);
        void addNodeItem(GenericNodeItem*);
        void addLinkItem(GenericLinkItem*);

        QList<InputTextItem*> getInputTextItems();
        QList<GenericNodeItem*> getNodeItems();
        QList<GenericLinkItem*> getLinkItems();

    public slots:
        void editorLostFocus(InputTextItem *item);

    signals:
        void textInserted(QGraphicsTextItem *item);
        void itemSelected(QGraphicsItem *item);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void keyPressEvent(QKeyEvent *event);

        void wheelEvent(QWheelEvent *event);

        void scaleView(qreal scaleFactor);

        void timerEvent(QTimerEvent *);

    private:
        Device* d;
        InputTextItem* textItem;
        QColor myTextColor;
        QFont myFont;

        QPointF down_pos;

        QList<InputTextItem*> input_text_items;
        QList<GenericNodeItem*> node_items;
        QList<GenericLinkItem*> link_items;

    };

}


#endif // SCENE2D_H
