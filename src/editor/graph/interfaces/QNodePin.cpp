#include "QNodePin.h"
#include "QNode.h"
#include "QNodePinValue.h"

#include "graph/basic/QBasicTempConnection.h"
#include "graph/basic/QBasicNodeConnection.h"

#include <QCoreApplication>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <cassert>

namespace
{
    class QNodePinDrawing
    {
    public:
        template<class T>
        QNodePinDrawing(T func)
        {
            func(_border_path, _content_path);
        }

        const QPainterPath& borderPath() const
        {
            return _border_path;
        }

        const QPainterPath& contentPath() const
        {
            return _content_path;
        }

    private:
        QPainterPath _border_path;
        QPainterPath _content_path;
    };

    static QNodePinDrawing FlowPinTemplate([](QPainterPath& content, QPainterPath& border)
    {
        auto bounding_rect = editor::QNodePin::pinBoundingRect();
        double pinX = bounding_rect.x();
        double pinY = bounding_rect.y();
        double width = bounding_rect.width();
        double height = bounding_rect.height();

        content.moveTo({ pinX,                  pinY });
        content.lineTo({ pinX + width / 2,      pinY });
        content.lineTo({ pinX + width,          pinY + height / 2 });
        content.lineTo({ pinX + width / 2,      pinY + height });
        content.lineTo({ pinX,                  pinY + height });
        content.lineTo({ pinX,                  pinY });

        // Its the same
        border = content;
    });
}

namespace
{
    QFont TextFont = QFont("Hack", 10, QFont::Weight::Normal);
    QColor TextColor = QColor::fromRgbF(0.7, 0.7, 0.7, 1.0);
    QFontMetrics TextMetrics = QFontMetrics(TextFont);
}

QRectF editor::QNodePin::pinBoundingRect()
{
    return { 0, 0, 10, 12 };
}

QPointF editor::QNodePin::pinBoundingRectCenter()
{
    return (pinBoundingRect().bottomRight() / 2.0);
}

editor::QNodePin::QNodePin(editor::QNode* parent)
    : QGraphicsObject(parent)
{
}

void editor::QNodePin::initialize()
{
    static constexpr double Spacing = 6;

    prepareGeometryChange();
    _rect = pinBoundingRect();

    QRectF node_rect = parentItem()->boundingRect();
    QRectF text_rect = TextMetrics.boundingRect(name());

    _rect.setHeight(text_rect.height());
    _rect.translate({ 0, -2 });

    _value_rect.setTop(_rect.top());
    _value_rect.setHeight(text_rect.height());

    if (pinType() == PinType::Input)
    {
        _rect.setWidth(node_rect.width() - Spacing * 2);
        _value_rect.setLeft(text_rect.width() + pinBoundingRect().width() + Spacing * 2);
        _value_rect.setRight(node_rect.width() - Spacing * 2);
    }
    else // type == PinType::Output
    {
        _rect.setLeft(-node_rect.width() + Spacing * 2 + pinBoundingRect().width());
        _rect.setWidth(node_rect.width() - Spacing * 2);

        _value_rect.setLeft(_rect.left());
        _value_rect.setRight(-text_rect.width() - (Spacing * 2));
    }
}

QRectF editor::QNodePin::boundingRect() const
{
    return _rect;
}

QRectF editor::QNodePin::valueBoundingRect() const
{
    return _value_rect;
}

bool editor::QNodePin::editable() const
{
    return false;
}

static QPen create_pen_from_color(QColor color, double alpha, double width)
{
    color.setAlphaF(alpha);
    return { color, width };
}

void editor::QNodePin::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    int n = 0;
    for (auto* child : childItems())
    {
        auto* val = dynamic_cast<editor::QNodePinValue*>(child);
        qDebug("%i", val != nullptr);
    }
}

void editor::QNodePin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Constants
    const PinType type = pinType();
    const QColor pin_color = color();

    QRectF text_rect = TextMetrics.boundingRect(name());

    // Text rendering
    if (type == PinType::Input)
    {
        text_rect.translate(pinBoundingRect().bottomRight());
        text_rect.translate({ 6, -1 });

        painter->setFont(TextFont);
        painter->setPen(TextColor);
        painter->drawText(text_rect, name());
    }
    else // type == PinType::Output
    {
        text_rect.translate(-(text_rect.width() + pinBoundingRect().width()), pinBoundingRect().height());
        text_rect.translate({ 2, -1 });

        painter->setFont(TextFont);
        painter->setPen(TextColor);
        painter->drawText(text_rect, name());
    }

    // Pin symbol rendering
    QPen symbol_border_pen = create_pen_from_color(pin_color, 0.9, 1);
    QPen symbol_border_shadow_pen = create_pen_from_color(pin_color, 0.4, 2);

    // Symbol draw
    painter->setPen(symbol_border_pen);
    painter->drawPath(FlowPinTemplate.borderPath());
    painter->setPen(symbol_border_shadow_pen);
    painter->drawPath(FlowPinTemplate.borderPath());

    painter->fillPath(FlowPinTemplate.contentPath(), symbol_border_shadow_pen.color());
}
