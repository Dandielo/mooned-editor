#include "QScriptedNodePin.h"
#include "QScriptedNodePinValue.h"
#include "QScriptedNode.h"
#include "QScriptedNodeEvent.h"

#include "graph/basic/QBasicNodeConnection.h"

#include <QCoreApplication>
#include <cassert>

editor::QScriptedNodePin::QScriptedNodePin(QScriptedNode* parent, PinType type, QString name)
    : QBasicNodePin{ parent }
    , _type{ type }
    , _name{ name }
    , _editable{ true }
{
    setParent(parent);
}

editor::QScriptedNodePin::~QScriptedNodePin()
{
}

void editor::QScriptedNodePin::initialize(Scripts::CScriptManager* script_manager)
{
    QNodePin::initialize();

    _script_manager = script_manager;
}

void editor::QScriptedNodePin::shutdown()
{
    delete _value;
}

void editor::QScriptedNodePin::connect(editor::QNodeConnection* connection)
{
    if (pinType() == PinType::Input)
    {
        auto* conn = qobject_cast<editor::QBasicNodeConnection*>(connection);
        auto pin = qobject_cast<editor::QScriptedNodePin*>(conn->outputPin());
        assert(pin != this);

        if (isConnected())
        {
            disconnectAll();
        }

        if (_value)
        {
            _value->setConnectedValue(pin->value());
        }
    }

    QBasicNodePin::connect(connection);

    // Send and update event to the parent node object
    QCoreApplication::postEvent(parent(), new editor::QScriptedNodeEvent{});
}

void editor::QScriptedNodePin::disconnect(QNodeConnection* connection)
{
    QBasicNodePin::disconnect(connection);

    if (pinType() == PinType::Input)
    {
        auto* conn = qobject_cast<editor::QBasicNodeConnection*>(connection);
        auto pin = qobject_cast<editor::QScriptedNodePin*>(conn->outputPin());
        assert(pin != this && pin != nullptr);

        if (_value)
        {
            assert(_value->connectedValue() == pin->value());
            _value->setConnectedValue(nullptr);
        }
    }

    // Send and update event to the parent node object
    QCoreApplication::postEvent(parent(), new editor::QScriptedNodeEvent{});
}

void editor::QScriptedNodePin::setValue(QScriptedNodePinValue* val)
{
    static constexpr double Spacing = 5;

    _value = val;

    if (_value != nullptr)
    {
        auto node_rect = parentItem()->boundingRect();
        auto pin_rect = boundingRect();
        //QRectF val_rect = { 0.0, 0.0, node_rect.width() - pin_rect.width() - (Spacing * 2), pin_rect.height() };

        if (_type == PinType::Input)
        {
            //_value->setPos(QPointF(Spacing, -pin_rect.height()) + pin_rect.bottomRight());
            _value->setBoundingRect(valueBoundingRect());
        }
        else // _type == PinType::Output
        {
            //_value->setPos(QPointF(-pin_rect.width() - Spacing / 2, -pin_rect.height()) + pin_rect.bottomRight());
            _value->setBoundingRect(valueBoundingRect());
        }
    }
}

editor::QScriptedNodePinValue* editor::QScriptedNodePin::value() const
{
    return _value;
}

QColor editor::QScriptedNodePin::color() const
{
    return _value ? _value->color() : Qt::white;
}

QString editor::QScriptedNodePin::name() const
{
    return _name;
}

editor::PinType editor::QScriptedNodePin::pinType() const
{
    return _type;
}

bool editor::QScriptedNodePin::editable() const
{
    return _editable && (pinType() == PinType::Output || connections().empty());
}

void editor::QScriptedNodePin::setEditable(bool val)
{
    _editable = val;
}

bool editor::QScriptedNodePin::canConnect(QNodePin* pin) const
{
    auto scripted_pin = qobject_cast<QScriptedNodePin*>(pin);

    bool result = scripted_pin != nullptr;
    if (result)
    {
        result = !((_value != nullptr) ^ (scripted_pin->_value != nullptr));
    }
    if (result && _value != nullptr)
    {
        result = _value->typeId() == scripted_pin->_value->typeId();
    }
    return result;
}
