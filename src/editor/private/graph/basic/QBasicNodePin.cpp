#include "QBasicNodePin.h"
#include "QBasicNodeConnection.h"
#include "QBasicTempConnection.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <cassert>

namespace
{
    static QColor DefaultConnectionColor = QColor::fromRgbF(0.9, 0.9, 0.9, 0.8);
    static QColor InvalidConnectionColor = QColor::fromRgbF(0.9, 0.3, 0.3, 0.8);
    static QColor SuccessfulConnectionColor = QColor::fromRgbF(0.3, 0.9, 0.3, 0.8);
}

editor::QBasicNodePin::QBasicNodePin(QNode* parent)
    : QNodePin(parent)
    , _connections()
    , _temp_connection(nullptr)
{

}

editor::QBasicNodePin::~QBasicNodePin()
{
    //assert(_connections.empty());
}

bool editor::QBasicNodePin::canConnect(QNodePin* pin) const
{
    return true;
}

bool editor::QBasicNodePin::isConnected() const
{
    return !_connections.empty();
}

void editor::QBasicNodePin::connect(editor::QNodeConnection* connection)
{
    auto* conn = qobject_cast<QBasicNodeConnection*>(connection);
    assert(conn != nullptr);

    if (!_connections.contains(conn))
    {
        _connections.append(conn);
    }
}

void editor::QBasicNodePin::disconnect(QNodeConnection* connection)
{
    auto* conn = qobject_cast<QBasicNodeConnection*>(connection);
    assert(conn != nullptr);

    int index = _connections.indexOf(conn);
    if (index != -1)
    {
        _connections.remove(index);
    }
}

void editor::QBasicNodePin::disconnectAll()
{
    auto conns = connections();
    for (auto conn : conns)
        conn->disconnect();
}

QVector<editor::QNodeConnection*> editor::QBasicNodePin::connections() const
{
    return _connections;
}

editor::QNodeConnection* editor::QBasicNodePin::newTempConnection()
{
    return new editor::QBasicTempConnection(this);
}

editor::QNodeConnection* editor::QBasicNodePin::newNodeConnection(QNodePin* pin)
{
    return new editor::QBasicNodeConnection(this, pin);
}

void editor::QBasicNodePin::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        disconnectAll();
    }
    else
    {
        _temp_connection = qobject_cast<QBasicTempConnection*>(newTempConnection());
        _temp_connection->setPos(parentItem()->mapToScene(this->pos()));

        // Add the item to the scene
        scene()->addItem(_temp_connection);
    }
}

void editor::QBasicNodePin::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_temp_connection)
    {
        QPointF event_pos = event->pos();
        QPointF scene_pos = mapToScene(event_pos);
        QGraphicsItem* item = scene()->itemAt(scene_pos, {});
        if (item)
        {
            QNodePin* pin = dynamic_cast<QNodePin*>(item);
            if (pin)
            {
                if (pin->pinType() != pinType() && canConnect(pin))
                {
                    auto conn = newNodeConnection(pin);
                    conn->updateConnection();
                    scene()->addItem(conn);
                }
            }
        }

        // Disconnect (delete) the temp connection
        _temp_connection->disconnect();
        _temp_connection = nullptr;
    }
}

void editor::QBasicNodePin::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_temp_connection)
    {
        _temp_connection->useCubicLine(false);

        // Update the color
        auto color = DefaultConnectionColor;

        // Update the pin point
        QPointF eventPos = event->pos();
        QPointF scenePos = mapToScene(eventPos);
        QGraphicsItem* item = scene()->itemAt(scenePos, {});

        if (item)
        {
            QNodePin* pin = dynamic_cast<QNodePin*>(item);
            if (pin && pin != this)
            {
                if (pin->pinType() == pinType() || !(canConnect(pin) && pin->canConnect(this)))
                {
                    color = InvalidConnectionColor;
                }
                else
                {
                    color = SuccessfulConnectionColor;
                    _temp_connection->useCubicLine(true);

                    // Update how we display the connection
                    QPointF temp = pin->mapToScene(QNodePin::pinBoundingRectCenter()) - mapToScene(QNodePin::pinBoundingRectCenter());
                    eventPos = temp + QNodePin::pinBoundingRectCenter();
                }
            }
        }

        _temp_connection->setEndPoint(eventPos);
        _temp_connection->setColor(color);
    }
}

