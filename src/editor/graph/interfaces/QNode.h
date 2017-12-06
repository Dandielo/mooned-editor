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
        virtual QRectF boundingRect() const;
        virtual QRectF minBoundingRect();

        // Node properties
        virtual QColor color() const = 0;
        virtual QString name() const = 0;
        virtual QString desc() const = 0;

        virtual QVector<QNodePin*> inputPins() const = 0;
        virtual QVector<QNodePin*> outputPins() const = 0;

        //// State interface
        //virtual void releaseNode() = 0;

    protected:
        virtual void calculatePinPositions();

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        // React to state changes
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    protected:
        // Node rect (resize-able)
        QRectF _rect;
        QRectF _min_rect;
        bool _dirty;

        //QRectF m_ResizeRect;

        //// Node color
        //bool m_Selected;

        //// Node Pins
        //QVector<QNodePin*> m_InputPins;
        //QVector<QNodePin*> m_OutputPins;

    private:
        //bool m_CanRename;
        //bool m_CanResize;

        //// Node event helpers
        //bool m_Resizing;
        //QPointF m_MouseLastPos;
    };
}
