#include "graph/interfaces/QNodePinValue.h"

namespace editor
{
    class QScriptedNodeProperty;
    class QScriptedNodePinValue : public QNodePinValue
    {
    public:
        static QScriptedNodePinValue* create(QScriptedNodeProperty* prop);

        virtual ~QScriptedNodePinValue() override = default;

        virtual QString toString() const = 0;

        virtual QScriptedNodePinValue* connectedValue() const;
        virtual void setConnectedValue(QScriptedNodePinValue* val);

        virtual QRectF boundingRect() const override;
        virtual void setBoundingRect(QRectF rect) override;

    protected:
        QRectF _rect;
        QScriptedNodePinValue* _connected_value = nullptr;
    };
}