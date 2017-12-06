#pragma once 
#include "graph/interfaces/QNode.h"
#include "scripts/CNativeScriptObject.h"

namespace editor
{
    class QScriptedNode : public QNode, public Scripts::CNativeScriptObject<QScriptedNode>
    {
        Q_OBJECT;
        M_SCRIPT_TYPE(QScriptedNode, "CNode");

    public:
        QScriptedNode(asIScriptObject* object);
        virtual ~QScriptedNode() override;

        void initialize(Scripts::CScriptManager* script_Manager);
        
        virtual QString name() const override;
        virtual void setName(QString value);

        virtual QString desc() const override;
        virtual void setDesc(QString value);

        virtual QColor color() const override;
        virtual void setColor(QColor value);

        virtual QVector<QNodePin*> inputPins() const override;
        virtual QVector<QNodePin*> outputPins() const override;

        virtual QNodePin* newInputPin(QString name);
        virtual QNodePin* newOutputPin(QString name);

        virtual QNodePin* getPin(PinType pin_type, QString name);

        virtual void removePin(QNodePin* pin);

    public:
        asITypeInfo* scriptType() const { return _type; }

    protected:
        virtual bool event(QEvent* ev) override;

    protected:
        Scripts::CScriptManager* _script_manager;
        asITypeInfo* _type;

        QString _name;
        QString _description;
        QColor _color;

        QVector<QNodePin*> _input_pins;
        QVector<QNodePin*> _output_pins;
    };

}
