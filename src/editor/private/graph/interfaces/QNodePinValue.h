#pragma once
#include <QGraphicsObject>

namespace editor
{
    class QGraphicsEditableItem;
    class QNodePin;

    class QNodePinValue : public QGraphicsItem
    {
    public:
        QNodePinValue();
        virtual ~QNodePinValue() override;

        virtual void setBoundingRect(QRectF rect) = 0;

        virtual QColor color() const = 0;
        virtual uint typeId() const = 0;

        virtual QVariant value() const = 0;
        virtual QVariant resolvedValue() const = 0;
        virtual void setValue(QVariant val) = 0;

        virtual void beginEdit() = 0;
        virtual void endEdit() = 0;

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev) override;

    protected:
        QGraphicsEditableItem* _edit_item;
    };
}
