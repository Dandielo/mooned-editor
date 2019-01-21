#include "QBasicNode.h"
#include "QBasicNodePin.h"

namespace editor
{

class QGenericNodePin : public QBasicNodePin
{
public:
    QGenericNodePin(QNode* parent, PinType type, QString name)
        : QBasicNodePin{ parent }
        , _type{ type }
        , _name{ std::move(name) }
    { }

    QColor color() const override
    {
        return Qt::white;
    }

    QString name() const override
    {
        return _name;
    }

    PinType pinType() const override
    {
        return _type;
    }

private:
    PinType _type;
    QString _name;
};

//////////////////////////////////////////////////////////////////////////

QBasicNode::QBasicNode(QString name)
    : QNode{ }
    , _name{ std::move(name) }
{
}

QBasicNode::~QBasicNode()
{
    for (QNodePin* pin : _input_pins)
    {
        pin->disconnectAll();
        delete pin;
    }
    for (QNodePin* pin : _output_pins)
    {
        pin->disconnectAll();
        delete pin;
    }
}

void QBasicNode::setColor(QColor color)
{
    _color = std::move(color);
}

void QBasicNode::setName(QString name)
{
    _name = std::move(name);
}

void QBasicNode::setDesc(QString description)
{
    _description = std::move(description);
}

void QBasicNode::addPin(editor::PinType type, QString name)
{
    if (type == PinType::Input)
    {
        _input_pins.append(new QGenericNodePin(this, type, std::move(name)));
    }
    else if(type == PinType::Output)
    {
        _output_pins.append(new QGenericNodePin(this, type, std::move(name)));
    }
}

editor::QNodePin* QBasicNode::getPin(editor::PinType pin_type, QString name)
{
    editor::QNodePin* result = nullptr;
    if (pin_type == PinType::Input)
    {
        for (const auto& pin : _input_pins)
        {
            if (pin->name() == name)
            {
                result = pin;
                break;
            }
        }
    }
    else // pin_type == PinType::Output
    {
        for (const auto& pin : _output_pins)
        {
            if (pin->name() == name)
            {
                result = pin;
                break;
            }
        }
    }
    return result;
}

QColor QBasicNode::color() const
{
    return _color;
}

QString QBasicNode::name() const
{
    return _name;
}

QString QBasicNode::desc() const
{
    return _description;
}

QVector<QNodePin*> QBasicNode::inputPins() const
{
    return _input_pins;
}

QVector<QNodePin*> QBasicNode::outputPins() const
{
    return _input_pins;
}

}
