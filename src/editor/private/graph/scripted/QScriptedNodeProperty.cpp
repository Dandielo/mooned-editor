#include "QScriptedNodeProperty.h"
#include "QScriptedNode.h"
#include "QScriptedNodePin.h"
#include "QScriptedNodePinValue.h"

#include "scripts/CScriptManager.h"

#include <angelscript.h>
#include <cassert>

editor::QScriptedNodeProperty::QScriptedNodeProperty(asITypeInfo* type, int subtype_id)
    : _type(type)
    , _subtype_id(subtype_id)
    , _node_pin(nullptr)
{
}

editor::QScriptedNodeProperty::~QScriptedNodeProperty()
{
}

void editor::QScriptedNodeProperty::initialize(Scripts::CScriptManager* manager, QScriptedNode* node, int index)
{
    auto* node_type = node->scriptType();

    auto type = manager->GetPropertyAttr(node_type, index, "type", "");
    if (type.empty() || (type != "in" && type != "out"))
    {
        // Add error message to the node
        return;
    }

    const char* prop_name;
    node_type->GetProperty(index, &prop_name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    // Get the real prop name
    auto name = manager->GetPropertyAttr(node_type, index, "name", prop_name);

    if (type == "in")
    {
        _node_pin = dynamic_cast<QScriptedNodePin*>(node->newInputPin(name.c_str()));
        _node_pin->setValue(QScriptedNodePinValue::create(this));
        _node_pin->value()->setParentItem(_node_pin);
    }
    if (type == "out")
    {
        _node_pin = dynamic_cast<QScriptedNodePin*>(node->newOutputPin(name.c_str()));
        _node_pin->setValue(QScriptedNodePinValue::create(this));
        _node_pin->value()->setParentItem(_node_pin);

        auto disabled = manager->GetPropertyAttr(node_type, index, "noedit", "");
        if (!disabled.empty() && disabled == "noedit")
        {
            _node_pin->setEditable(false);
        }
    }

    assert(_node_pin != nullptr);
}

void editor::QScriptedNodeProperty::copyFrom(const QScriptedNodeProperty& other)
{
    _node_pin->value()->setValue(other.nodePin()->value()->resolvedValue());
}
