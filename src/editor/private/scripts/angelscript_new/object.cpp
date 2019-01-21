#include <scripts/angelscript_new/object.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{

Object::Object(ConstructorData<Object> data) noexcept
    : _type{ data.type }
    , _script_address{ data.object }
{ }

//////////////////////////////////////////////////////////////////////////

ScriptObject::ScriptObject(ConstructorData<ScriptObject> data) noexcept
    : _type{ data.type }
    , _script_object{ data.object }
{
    _script_object->AddRef();
}

ScriptObject::ScriptObject(ScriptObject&& other) noexcept
    : _type{ other._type }
    , _script_object{ other._script_object }
{
    other._script_object = nullptr;
}

ScriptObject& ScriptObject::operator=(ScriptObject&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (_script_object)
    {
        _script_object->Release();
    }

    _type = other._type;
    _script_object = other._script_object;

    other._script_object = nullptr;
    return *this;
}

ScriptObject::ScriptObject(const ScriptObject& other) noexcept
{
}

ScriptObject& ScriptObject::operator=(const ScriptObject& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (_script_object)
    {
        _script_object->Release();
    }

    _type = other._type;
    _script_object = other._script_object;

    if (_script_object)
    {
        _script_object->AddRef();
    }
    return *this;
}

ScriptObject::~ScriptObject() noexcept
{
    if (_script_object)
    {
        _script_object->Release();
    }
}

auto ScriptObject::from_native(asIScriptObject* native_script_object) noexcept -> ScriptObject
{
    return ConstructorData<ScriptObject>{ Type::from_native(native_script_object->GetObjectType()), native_script_object };
}

} // namespace editor::script
