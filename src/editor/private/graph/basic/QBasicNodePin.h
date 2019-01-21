#pragma once
#include "graph/interfaces/QNodePin.h"

#include <QVector>

namespace editor
{
    class QBasicNodeConnection;
    class QBasicTempConnection;

    class QBasicNodePin : public QNodePin
    {
        Q_OBJECT;

    public:
        QBasicNodePin(QNode* parent);
        virtual ~QBasicNodePin() override;

        // Connections
        virtual bool canConnect(QNodePin* pin) const override;
        virtual bool isConnected() const override;
        virtual void connect(QNodeConnection* connection) override;
        virtual void disconnect(QNodeConnection* connection) override;
        virtual void disconnectAll() override;

        virtual QVector<QNodeConnection*> connections() const override;

        // Factories
        virtual QNodeConnection* newTempConnection() override;
        virtual QNodeConnection* newNodeConnection(QNodePin* pin) override;

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    protected:
        QVector<QNodeConnection*> _connections;
        QBasicTempConnection* _temp_connection;
    };
}
