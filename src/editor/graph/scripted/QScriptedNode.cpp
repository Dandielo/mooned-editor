#include "QScriptedNode.h"
#include "QScriptedNodePin.h"
#include "QScriptedNodeProperty.h"

#include "scripts/CScriptManager.h"

#include <QFont>
#include <QEvent>
#include <cassert>

editor::QScriptedNode::QScriptedNode(asIScriptObject* object) : QNode(), CNativeScriptObject(object)
{
    _type = object->GetObjectType();

    // The node name
    //m_Name.setDefaultTextColor(textColor);
    //m_Name.setPlaceholderText("No name");
    //m_Name.setFont(nameFont);
    //m_Name.setFlags(m_Name.flags() & ~(QGraphicsItem::ItemIsFocusable));
    //m_Name.setFinishCallback([&]() {
    //    m_RectDirty = true;
    //    resize();
    //    calculatePinPositions();
    //});
    //m_Name.setTextChangedCallback([&](int) -> bool {
    //    m_RectDirty = true;
    //    resize();
    //    calculatePinPositions();
    //    return true;
    //});
    //m_Name.setTextWidth(m_Rect.width() - 4);

    //// Default name
    //setName(QString::fromStdString(script_manager->GetTypeAttr(typeinfo, "name", typeinfo->GetName())));

    //// The node description
    //m_Description.setDefaultTextColor(textShadow);
    //m_Description.setPlaceholderText("Test");
    //m_Description.setFont(descriptionFont);
    //m_Description.setFlags(m_Description.flags() & ~(QGraphicsItem::ItemIsFocusable));
    //m_Description.setFinishCallback([&]() {
    //    m_RectDirty = true;
    //    resize();
    //    calculatePinPositions();
    //});
    //m_Description.setTextChangedCallback([&](int) -> bool {
    //    m_RectDirty = true;
    //    resize();
    //    calculatePinPositions();
    //    return true;
    //});
    //m_Description.setTextWidth(m_Rect.width() - 9);
    //m_Description.setPos(5, 0);


    // Last changes
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsScenePositionChanges);
    setCacheMode(QGraphicsItem::CacheMode::ItemCoordinateCache);
    setZValue(2.0);

    //// Default bounding rect
    //qreal penWidth = 2;
    //m_Rect = QRectF(-penWidth / 2.0, -penWidth / 2.0, 20 + penWidth, 20 + penWidth);
    //m_ResizeRect = QRectF(m_Rect.right() - 10, m_Rect.bottom() - 10, 10, 10);


    //float height = m_Description.boundingRect().height();
    //height += m_Name.boundingRect().height();

    //// Event values
    //m_Resizing = false;
    //m_RectDirty = true;
}

editor::QScriptedNode::~QScriptedNode()
{
    CallScriptMethod("OnDestroy");

    for (QNodePin* pin : _input_pins)
    {
        pin->disconnectAll();
        pin->deleteLater();
    }
    for (QNodePin* pin : _output_pins)
    {
        pin->disconnectAll();
        pin->deleteLater();
    }
}

void editor::QScriptedNode::initialize(Scripts::CScriptManager* script_Manager)
{
    QNode::initialize();

    _script_manager = script_Manager;
    //auto* pin = newInputPin("");
    //pin = newOutputPin("");
    //pin = newInputPin("");
    //pin = newOutputPin("");
    //pin = newInputPin("");
    //pin = newOutputPin("");

    //// Check if we can change some node values on the fly
    //m_CanRename = !script_manager->GetTypeAttr(typeinfo, "allow_rename").empty();
    //m_CanResize = !script_manager->GetTypeAttr(typeinfo, "allow_resize").empty();

    //// Default description
    setName(QString::fromStdString(_script_manager->GetTypeAttr(_type, "name")));
    setDesc(QString::fromStdString(_script_manager->GetTypeAttr(_type, "desc")));

    // Get type properties
    uint property_count = _type->GetPropertyCount();
    for (uint i = 0; i < property_count; ++i)
    {
        auto* prop = reinterpret_cast<editor::QScriptedNodeProperty*>(_script_object->GetAddressOfProperty(i));
        assert(prop != nullptr);

        // Initialize the property
        prop->initialize(_script_manager, this, i);
    }

    //// Script defined properties
    //int property_count = typeinfo->GetPropertyCount();
    //for (int i = 0; i < property_count; ++i)
    //{
    //    int property_type_id;
    //    bool is_reference;
    //    int offset;
    //    const char* name;
    //    typeinfo->GetProperty(i, &name, &property_type_id, nullptr, nullptr, &offset, &is_reference);
    //    auto property_type = typeinfo->GetEngine()->GetTypeInfoById(property_type_id);

    //    auto value = script_manager->GetPropertyAttr(typeinfo, i, "type");
    //    if (value == "in")
    //    {
    //        auto pin = newInputPin(name + 2);
    //        auto* prop = reinterpret_cast<QNodeProperty*>(obj->GetAddressOfProperty(i));
    //        prop->initialize(pin, property_type->GetSubTypeId(0), property_type->GetSubType(0));
    //    }
    //    else
    //        if (value == "out")
    //        {
    //            auto pin = reinterpret_cast<QNodeOutputPin*>(newOutputPin(name + 2));
    //            auto* prop = reinterpret_cast<QNodeProperty*>(obj->GetAddressOfProperty(i));
    //            prop->initialize(pin, property_type->GetSubTypeId(0), property_type->GetSubType(0));

    //            // Check if the output pin should be always editable (aka. is not set in the script)
    //            if (script_manager->GetPropertyAttr(typeinfo, i, "editable") == "editable")
    //                pin->setEditable(true);
    //        }

    //    is_reference = false;
    //}

    //// Resize the node
    //resize();

    _dirty = true;
    _rect = minBoundingRect();

    // and calculate the pin positions
    calculatePinPositions();

    // Call the script callback
    CallScriptMethod("OnCreate");
}

QString editor::QScriptedNode::name() const
{
    return _name;
}

void editor::QScriptedNode::setName(QString value)
{
    _name = value;
}

QString editor::QScriptedNode::desc() const
{
    return _description;
}

void editor::QScriptedNode::setDesc(QString value)
{
    _description = value;
}

QColor editor::QScriptedNode::color() const
{
    return _color;
}

void editor::QScriptedNode::setColor(QColor value)
{
    _color = value.darker(140);
    _color.setAlphaF(0.9f);
}

QVector<editor::QNodePin*> editor::QScriptedNode::inputPins() const
{
    return _input_pins;
}

QVector<editor::QNodePin*> editor::QScriptedNode::outputPins() const
{
    return _output_pins;
}

editor::QNodePin* editor::QScriptedNode::newInputPin(QString name)
{
    auto* pin = new editor::QScriptedNodePin(this, PinType::Input, name); // new editor::QNodeInputPin;
    pin->initialize(_script_manager);
    _input_pins.push_back(pin);
    return pin;
}

editor::QNodePin* editor::QScriptedNode::newOutputPin(QString name)
{
    auto* pin = new editor::QScriptedNodePin(this, PinType::Output, name); // new editor::QNodeInputPin;
    pin->initialize(_script_manager);
    _output_pins.push_back(pin);
    return pin;
}

editor::QNodePin* editor::QScriptedNode::getPin(PinType pin_type, QString name)
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

void editor::QScriptedNode::removePin(editor::QNodePin* pin)
{
    if (pin->pinType() == PinType::Input)
    {
        _input_pins.removeAt(_input_pins.indexOf(pin));
    }
    else // pin->pinType() == PinType::Output
    {
        _output_pins.removeAt(_input_pins.indexOf(pin));
    }
}

bool editor::QScriptedNode::event(QEvent* event)
{
    //if (event->type() == QNodePinValueEvent::EventType)
    //{
    //    CallScriptMethod("Update");
    //    return true;
    //}
    return QNode::event(event);
}
