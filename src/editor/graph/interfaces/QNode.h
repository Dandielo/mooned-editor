#pragma once
#include "QNodePin.h"

#include <QGraphicsItem>

namespace editor
{
    class QNode : public QGraphicsObject
    {
        Q_OBJECT;

    public:
        QNode();
        virtual ~QNode() override = default;

        virtual void initialize();

        // Node const attributes
        virtual QRectF boundingRect() const override;
        virtual QRectF minBoundingRect();

        // Node properties
        virtual QColor color() const = 0;
        virtual QString name() const = 0;
        virtual QString desc() const = 0;

        virtual QVector<QNodePin*> inputPins() const = 0;
        virtual QVector<QNodePin*> outputPins() const = 0;

    protected:
        virtual void calculatePinPositions();

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        // React to state changes
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    protected:
        QRectF _rect;
        QRectF _min_rect;
        bool _dirty;
    };
}
