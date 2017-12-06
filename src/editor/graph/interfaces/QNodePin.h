#pragma once
#include <QGraphicsItem>

namespace editor
{
    enum class PinType { Input, Output };

    class QNode;
    class QNodeConnection;
    class QNodeConnectionEvent;

    class QNodePin : public QGraphicsObject
    {
        Q_OBJECT;

    public:
        static QRectF pinBoundingRect();
        static QPointF pinBoundingRectCenter();

        QNodePin(QNode* parent);
        virtual ~QNodePin() override = default;

        virtual void initialize();

        virtual QRectF boundingRect() const override;
        virtual QRectF valueBoundingRect() const;

        virtual QColor color() const = 0;
        virtual QString name() const = 0;
        virtual PinType pinType() const = 0;
        virtual bool editable() const;

        // Connections
        virtual bool canConnect(QNodePin* pin) const = 0;
        virtual bool isConnected() const = 0;
        virtual void connect(QNodeConnection* connection) = 0;
        virtual void disconnect(QNodeConnection* connection) = 0;
        virtual void disconnectAll() = 0;

        virtual QVector<QNodeConnection*> connections() const = 0;

        // Factories
        virtual QNodeConnection* newTempConnection() = 0;
        virtual QNodeConnection* newNodeConnection(QNodePin* pin) = 0;

    protected:
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    private:
        QRectF _rect;
        QRectF _value_rect;
    };
}
