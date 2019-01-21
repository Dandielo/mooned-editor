#include "QNodePinValue.h"
#include "QNodePin.h"

#include "graph/utils/QGraphicsEditableItem.h"

#include <QGraphicsSceneMouseEvent>

editor::QNodePinValue::QNodePinValue()
    : QGraphicsItem(nullptr)
    , _edit_item(nullptr)
{
}

editor::QNodePinValue::~QNodePinValue()
{
    if (_edit_item)
    {
        delete _edit_item;
        _edit_item = nullptr;
    }
}

void editor::QNodePinValue::mousePressEvent(QGraphicsSceneMouseEvent* ev)
{
    if (_edit_item)
    {
        QGraphicsItem::mousePressEvent(ev);
    }
    else
    {
        /* needs to be reimplemented to accept double clicks (lol?) */
    }
}

void editor::QNodePinValue::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev)
{
    qDebug("QNodePinValue::mouseDoubleClickEvent()");
    auto* pin = dynamic_cast<QNodePin*>(parentItem());

    if (pin->editable() && _edit_item == nullptr)
    {
        beginEdit();
    }
}
