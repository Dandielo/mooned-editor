#include "graph/interfaces/QNodeConnection.h"

namespace editor
{
    class QNodePin;

    class QBasicTempConnection : public QNodeConnection
    {
        Q_OBJECT;

    public:
        QBasicTempConnection(QNodePin* pin);
        virtual ~QBasicTempConnection() override = default;

        virtual QRectF boundingRect() const;

        virtual void setColor(QColor color);
        virtual void setEndPoint(QPointF pos);
        virtual void useCubicLine(bool value);

        virtual void updatePath() override;
        virtual void updateConnection() override;
        virtual void disconnect() override;
        
    private:
        QNodePin* _pin;
        QPointF _end_point;
        bool _cubic_line;
    };
}
