#include "QBasicNodeConnection.h"
#include "graph/interfaces/QNodePin.h"

#include <QPen>

#include <cassert>

editor::QBasicNodeConnection::QBasicNodeConnection(QNodePin* pin1, QNodePin* pin2) 
    : QNodeConnection()
    , _input_pin(nullptr)
    , _output_pin(nullptr)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

    assert(pin1->pinType() != pin2->pinType());
    if (pin1->pinType() == editor::PinType::Output)
    {
        _output_pin = pin1;
        _input_pin = pin2;
    }
    else
    {
        _output_pin = pin2;
        _input_pin = pin1;
    }

    _output_pin->connect(this);
    _input_pin->connect(this);
}

QRectF editor::QBasicNodeConnection::boundingRect() const
{
    QPointF p1 = _output_pin->mapToScene(QNodePin::pinBoundingRectCenter());
    QPointF p2 = _input_pin->mapToScene(QNodePin::pinBoundingRectCenter());
    QPointF p3 = p2 - p1 + QNodePin::pinBoundingRectCenter();

    return { QNodePin::pinBoundingRectCenter(), p3 };
}

QPainterPath editor::QBasicNodeConnection::shape() const
{
    static QPainterPathStroker stroke(QPen(Qt::black, 12));
    return stroke.createStroke(_path);
}

void editor::QBasicNodeConnection::updateConnection()
{
    prepareGeometryChange();
    setPos(_output_pin->parentItem()->mapToScene(_output_pin->pos()));

    updatePath();

    // Set the output pin color
    _color = _output_pin->color();
}

editor::QNodePin* editor::QBasicNodeConnection::inputPin() const
{
    return _input_pin;
}

editor::QNodePin* editor::QBasicNodeConnection::outputPin() const
{
    return _output_pin;
}

void editor::QBasicNodeConnection::disconnect()
{
    _output_pin->disconnect(this);
    _input_pin->disconnect(this);
    deleteLater();
}
