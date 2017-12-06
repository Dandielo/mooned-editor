#include "QNodeConnection.h"

#include <QPainter>

editor::QNodeConnection::QNodeConnection() 
    : _path()
    , _color(QColor::fromRgbF(0.9, 0.9, 0.9, 0.8))
{
    setZValue(-2.0);
}

void editor::QNodeConnection::updatePath()
{
    QRectF rect = boundingRect();
    QPointF cp1 = rect.topLeft();
    QPointF cp2 = rect.bottomRight();

    cp1 += { 80, 0 };
    cp2 -= { 80, 0 };

    qreal ratio = cp1.x() - cp2.x();
    if (ratio > 0)
    {
        ratio /= 2;
        cp1 += { ratio, 0.0 };
        cp2 -= { ratio, 0.0 };
    }

    _path = QPainterPath();
    _path.moveTo(rect.topLeft());
    _path.cubicTo(cp1, cp2, rect.bottomRight());
}

editor::QNodePin* editor::QNodeConnection::inputPin() const
{
    return nullptr;
}

editor::QNodePin* editor::QNodeConnection::outputPin() const
{
    return nullptr;
}

void editor::QNodeConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QColor outline = isSelected() ? Qt::white : _color;
    outline.setAlphaF(0.4);
    
    painter->setPen(QPen(_color, 2));
    painter->drawPath(_path);
    painter->setPen(QPen(outline, 3));
    painter->drawPath(_path);
}
