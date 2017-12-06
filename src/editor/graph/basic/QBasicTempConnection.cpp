#include "QBasicTempConnection.h"
#include "graph/interfaces/QNodePin.h"

editor::QBasicTempConnection::QBasicTempConnection(QNodePin* pin) 
    : QNodeConnection()
    , _pin(pin)
    , _end_point(editor::QNodePin::pinBoundingRectCenter())
    , _cubic_line(false)
{
}

void editor::QBasicTempConnection::setColor(QColor color)
{
    _color = color;
}

void editor::QBasicTempConnection::setEndPoint(QPointF pos)
{
    _end_point = pos;

    prepareGeometryChange();
    updateConnection();
}

void editor::QBasicTempConnection::useCubicLine(bool value)
{
    _cubic_line = value;
}

QRectF editor::QBasicTempConnection::boundingRect() const
{
    return { QNodePin::pinBoundingRectCenter(), _end_point };
}

void editor::QBasicTempConnection::updatePath()
{
    QRectF rect = boundingRect();
    QPointF cp1 = rect.topLeft();
    QPointF cp2 = rect.bottomRight();

    if (_cubic_line)
    {
        if (_pin->pinType() == editor::PinType::Input)
        {
            cp1 -= { 80, 0 };
            cp2 += { 80, 0 };

            qreal ratio = cp2.x() - cp1.x();
            if (ratio > 0)
            {
                ratio /= 2;
                cp1 -= { ratio, 0.0 };
                cp2 += { ratio, 0.0 };
            }
        }
        if (_pin->pinType() == editor::PinType::Output)
        {
            cp1 += { 80, 0 };
            cp2 -= { 80, 0 };

            qreal ratio = cp1.x() - cp2.x();
            if (ratio > 0)
            {
                ratio /= 2;
                cp1 += { ratio, 0.0 };
                cp2 -= { ratio, 0.0 };
            }
        }

        _path = QPainterPath();
        _path.moveTo(rect.topLeft());
        _path.cubicTo(cp1, cp2, rect.bottomRight());
    }
    else
    {
        auto startPoint = QNodePin::pinBoundingRectCenter();
        auto endPoint = _end_point;

        if (_pin->pinType() == editor::PinType::Input)
        {
            cp1 = startPoint;
            cp2 = endPoint;

            cp1 += { -abs(startPoint.x() - endPoint.x()) / 3.0 - 50, 0 };
        }
        if (_pin->pinType() == editor::PinType::Output)
        {
            cp1 = startPoint;
            cp2 = endPoint;

            cp1 += { abs(startPoint.x() - endPoint.x()) / 3.0 + 50, 0 };
        }

        _path = QPainterPath();
        _path.moveTo(startPoint);
        _path.cubicTo(cp1, cp2, endPoint);
    }
}

void editor::QBasicTempConnection::updateConnection()
{
    updatePath();
}

void editor::QBasicTempConnection::disconnect()
{
    deleteLater();
}
