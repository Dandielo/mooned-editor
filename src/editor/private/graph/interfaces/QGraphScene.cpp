#include "QGraphScene.h"
#include <QGraphicsItem>
#include <QEvent>

editor::QGraphScene::QGraphScene() : QGraphScene(QRectF())
{

}

editor::QGraphScene::QGraphScene(const QRectF& rect) : QGraphicsScene(rect)
{

}

bool editor::QGraphScene::event(QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        //emit focused();
    }
    if (event->type() == QEvent::Paint)
    {
        for (QGraphicsItem* item : selectedItems())
        {
            item->update();
        }
    }

    return QGraphicsScene::event(event);
}
