#include "graph/interfaces/QNodeConnection.h"

namespace editor
{
    class QNodePin;

    class QBasicNodeConnection : public QNodeConnection
    {
        Q_OBJECT;

    public:
        QBasicNodeConnection(QNodePin* pin1, QNodePin* pin2);
        virtual ~QBasicNodeConnection() override = default;

        virtual QRectF boundingRect() const override;
        virtual QPainterPath shape() const override;

        virtual void updateConnection() override;

        virtual QNodePin* inputPin() const override;
        virtual QNodePin* outputPin() const override;

        virtual void disconnect() override;

    private:
        QNodePin* _input_pin;
        QNodePin* _output_pin;
    };
}
