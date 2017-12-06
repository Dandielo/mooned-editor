#pragma once
#include "graph/basic/QBasicNodePin.h"
#include "scripts/CScriptManager.h"

namespace editor
{
    class QScriptedNode;
    class QScriptedNodePinValue;

    class QScriptedNodePin : public QBasicNodePin
    {
        Q_OBJECT;

    public:
        QScriptedNodePin(QScriptedNode* parent, PinType type, QString name);
        virtual ~QScriptedNodePin() override;

        void initialize(Scripts::CScriptManager* script_manager);

        virtual void connect(editor::QNodeConnection* connection) override;
        virtual void disconnect(QNodeConnection* connection) override;

        virtual void setValue(QScriptedNodePinValue* val);
        virtual QScriptedNodePinValue* value() const;

        virtual QColor color() const override;
        virtual QString name() const override;
        virtual PinType pinType() const override;
        virtual bool editable() const override;

        virtual bool canConnect(QNodePin* pin) const override;

    private:
        const PinType _type;
        const QString _name;

        Scripts::CScriptManager* _script_manager;
        QScriptedNodePinValue* _value;
    };
}
