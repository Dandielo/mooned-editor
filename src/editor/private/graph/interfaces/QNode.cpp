#include "QNode.h"
#include "QNodeConnection.h"

#include <QFont>
#include <QFontMetrics>
#include <QPen>

#include <QPainter>
#include <QPaintEngine>

namespace
{
    QFont NameFont = QFont("Hack", 11, QFont::Weight::Normal);
    QFont DescriptionFont = QFont("Hack", 9, 1, true);
    QFontMetrics NameMetrics = QFontMetrics(NameFont);
    QFontMetrics DescriptionMertics = QFontMetrics(DescriptionFont);
    QColor TextColor = QColor::fromRgbF(0.9, 0.9, 0.9, 1.0);
    QColor TextShadow = QColor::fromRgbF(0.9, 0.9, 0.9, 0.4);
}

editor::QNode::QNode()
    : QGraphicsObject(nullptr)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsScenePositionChanges);
}

void editor::QNode::initialize()
{
    _rect = minBoundingRect();
}

QRectF editor::QNode::boundingRect() const
{
    return _rect;
}

QRectF editor::QNode::minBoundingRect()
{
    if (_dirty)
    {
        _dirty = false;
        _min_rect = _rect;

        // Calculate width
        int min_width = 180;
        bool two_side_pins = (!inputPins().empty() && !outputPins().empty());
        if (two_side_pins)
            min_width *= 1;
        _min_rect.setWidth(min_width);

        // Calculate height
        QRectF name_rect = NameMetrics.boundingRect(name());
        QRectF desc_rect = DescriptionMertics.boundingRect(desc());

        int height = 5;
        height += name_rect.height();
        height += desc_rect.height();

        height += (inputPins().size() + outputPins().size()) * (QNodePin::pinBoundingRect().height() * 1.5) + 8;

        _min_rect.setHeight(height);
    }
    return _min_rect;
}

void editor::QNode::calculatePinPositions()
{
    auto nodeRect = boundingRect();
    auto barRect = nodeRect;

    // Calculate height
    QRectF name_rect = NameMetrics.boundingRect(name());
    QRectF desc_rect = DescriptionMertics.boundingRect(desc());

    int height = 5;
    height += name_rect.height();
    height += desc_rect.height();

    // Set pins 
    double x_pos = boundingRect().width() - QNodePin::pinBoundingRect().width() - 6;
    double y_pos = height + 6;
    for (QNodePin* pin : outputPins())
    {
        pin->setPos(x_pos, y_pos);
        y_pos += pin->pinBoundingRect().height() * 1.5;
    }

    x_pos = 6;
    for (QNodePin* pin : inputPins())
    {
        pin->setPos(x_pos, y_pos);
        y_pos += QNodePin::pinBoundingRect().height() * 1.5;
    }
}

QVariant editor::QNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged)
    {
        // Update connections
        for (auto& pin : outputPins())
        {
            for (auto& con : pin->connections())
            {
                con->updateConnection();
            }
        }
        for (auto& pin : inputPins())
        {
            for (auto& con : pin->connections())
            {
                con->updateConnection();
            }
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void editor::QNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainter& p = *painter;
    p.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);

    static const QColor staticBarColor = QColor::fromRgbF(0.15, 0.15, 0.15, 0.95);	// dark gray
    static const QColor staticBorderColor = QColor::fromRgbF(0.1, 0.1, 0.1, 1.0);	// black
    static const QColor staticBorderShadow = QColor::fromRgbF(0.0, 0.0, 0.0, 0.3);	// black with alpha

    // Get static data
    auto node_color = color();
    auto node_rect = boundingRect();
    auto bar_rect = node_rect;

    QRectF name_rect = NameMetrics.boundingRect(name());
    QRectF desc_rect = DescriptionMertics.boundingRect(desc());

    // One time calculations 
    QPointF name_pos = { 4, name_rect.height() - 1 };
    QPointF desc_pos = { 6, name_pos.y() + desc_rect.height() + 1 };

    // Set the bar height 
    bar_rect.setHeight(desc_rect.height() + name_rect.height() + 5);

    // Node rect
    p.setPen(QPen(staticBorderColor, 1));
    p.setBrush(staticBarColor);
    p.drawRect(node_rect);

    // Bar
    QLinearGradient gradient(0.0, 0.0, bar_rect.width(), bar_rect.height());
    gradient.setColorAt(0.0, node_color);
    gradient.setColorAt(1.0, node_color.darker());

    p.setBrush(gradient);
    p.drawRect(bar_rect);

    // Bar text
    auto old_font = p.font();
    p.setPen(TextColor);
    p.setFont(NameFont);
    p.drawText(name_pos, name());
    p.setPen(TextShadow);
    p.setFont(DescriptionFont);
    p.drawText(desc_pos, desc());
    p.setFont(old_font);

    // Border drawing
    QColor borderColor = staticBorderColor;
    if (isSelected())
        borderColor = QColor::fromRgbF(0.8, 0.8, 0.8, 1.0);

    // Border Shadow
    p.setBrush(::Qt::NoBrush);
    p.setPen(QPen(borderColor, 2, Qt::SolidLine, Qt::PenCapStyle::FlatCap, Qt::PenJoinStyle::MiterJoin));
    p.drawRect(node_rect);
}
